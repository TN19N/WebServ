// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/23/23 at 3:01 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static void __dot_dot_handler_last_dir_(const char *path, size_t len, std::string &body)
{
	const char *last = path + len - 2; // -2 for skipp / at the end
	
	while (path < last && *last != '/')
		--last;
	if (last > path)
		while (path < last)
			body.append(1, *path++) ;
	body.append(1, '/') ;
}

static void __do_response_with_directory_content_(Client* client)
{
	Response		*response;
	std::string		body;
	std::string		&path = client->getRequest()->path;
	DIR* 			dir;
	struct dirent*	ent;
	
	response = new Response(200, client->getRequest()->keepAlive);
	client->setResponse(response);
	
	if ((dir = opendir(client->getRequest()->fullPath.c_str())) == nullptr)
		throw 403;
	body = " <!DOCTYPE html>\n<html>\n<head>\n<title>";
	body.append(path);
	body.append("</title>\n</head>\n<body>\n<h1>Directory Content</h1>\n");
	while ((ent = readdir(dir)))
	{
		body.append("<p><a href=\"");
		if (strcmp(ent->d_name, ".") == 0)
			body.append(path) ;
		else if (strcmp(ent->d_name, "..") == 0)
			__dot_dot_handler_last_dir_(path.c_str(), path.size(), body);
		else
		{
			body.append(path);
			body.append(ent->d_name) ;
			if (ent->d_type == DT_DIR)
				body.append(1, '/');
		}
		body.append("\">");
		body.append(ent->d_name);
		if (ent->d_type == DT_DIR)
			body.append(1, '/');
		body.append("</a></p>\n");
	}
	body.append("</body>\n</html>\n");
	closedir(dir);
	response->addHeader("Content-Type", "text/html");
	response->addBody(body);
	client->switchState();
}

static void __read_file_content_to_do_response_(Client* client) {
	std::map<std::string, std::vector<std::string> >::const_iterator	directive, notFound;
	Response		*response;
	struct stat		pathInfo;
	
	response = new Response(200, client->getRequest()->keepAlive);
	client->setResponse(response);
	
	stat(client->getRequest()->fullPath.c_str(), &pathInfo);
	if ((response->download_file_fd = open(client->getRequest()->fullPath.c_str(), O_RDONLY)) < 0) {
		throw 403;
	}

	notFound = client->getRequest()->location->getDirectives().end();
	directive = client->getRequest()->location->getDirectives().find(client->getRequest()->extension);
	if (directive != notFound) {
		response->addHeader("Content-Type", directive->second[0]);
	} else {
		response->addHeader("Content-Type", DEFAULT_MIME_TYPE);
	}
	response->addHeader("Content-Length",std::to_string(pathInfo.st_size));
	response->buffer.append(CRLF);
	client->switchState();
}

void HTTP::getMethodHandler(Client *client)
{
	std::map<std::string, std::vector<std::string> >::const_iterator	directive, notFound;
	std::vector<std::string>::const_iterator							begin, end;
	
	Request					*request = client->getRequest();
	struct stat				pathInfo;

	notFound = request->location->getDirectives().end();
	// Check for redirection
	directive = request->location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != notFound) {
		throw std::make_pair(std::stoi(directive->second[0]), directive->second[1]);
	}

	if (stat(request->fullPath.c_str(), &pathInfo) < 0) {
		throw 404;
	}
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode)) {
		if (request->path.c_str()[request->path.size()-1] != '/') {
			throw std::make_pair(301, request->path + '/');
		} else {
			directive = request->location->getDirectives().find(INDEX_DIRECTIVE);
			for (begin = directive->second.begin(), end = directive->second.end(); begin != end; ++begin) {
				if (access((request->fullPath + *begin).c_str(), F_OK) == 0) {
					request->fullPath.append(*begin);
					request->path.append(*begin);
					request->extension = HTTP::getExtensionFromPath(request->path.c_str());
					break;
				}
			}
			if (begin == end) {
				if (request->location->getDirectives().find(AUTOINDEX_DIRECTIVE)->second[0] == "off") {
					throw 403;
				} else {
					__do_response_with_directory_content_(client);
				}
				return ;
			}
		}
	}
	// Check for CGI
	directive = request->location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != notFound) {
		for (begin = directive->second.begin(), ++begin, end = directive->second.end(); begin < end; ++begin, ++begin)
			if (*begin == request->extension)
			{
				--begin;
				throw 1337;
			}
	}
	// no CGI just a simple file
	__read_file_content_to_do_response_(client);
}
