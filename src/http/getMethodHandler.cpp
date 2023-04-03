# include <string.h>
# include <sstream>

# include "webserv/context.hpp"
# include "webserv/http.hpp"

static void __do_response_with_directory_content_(Client* client)
{
	Response*		response;
	std::string		body;
	DIR* 			dir;
	struct dirent*	ent;
	std::string&	path = client->getRequest()->path;

	response = new Response(200, client->getRequest()->keepAlive);
	client->setResponse(response);

	if ((dir = opendir(client->getRequest()->fullPath.c_str())) == NULL)
		throw 403;

	body = " <!DOCTYPE html>\n<html>\n<head>\n<title>";
	body.append(path);
	body.append("</title>\n</head>\n<body>\n<h1>Directory Content</h1>\n");
	while ((ent = readdir(dir)))
	{
		body.append("<h4><a href=\"");
		if (strcmp(ent->d_name, ".") == 0) {
			body.append(path) ;
		}
		else if (strcmp(ent->d_name, "..") == 0) {
			int	i;
			int	loc_size = client->getRequest()->location->getArgs().at(0).size();

			for (i = path.size() - 2; path[i] != '/' && loc_size < i; --i) {}
			if (i <= loc_size) {
				i = loc_size == 1 ? 0 : loc_size;
			}
			body.append(path, 0, i + 1);
		} else {
			body.append(path);
			body.append(ent->d_name) ;
			if (ent->d_type == DT_DIR)
				body.append(1, '/');
		}
		body.append("\">");
		body.append(ent->d_name);
		if (ent->d_type == DT_DIR)
			body.append(1, '/');
		body.append("</a></h4>\n");
	}
	body.append("</body>\n</html>\n");
	closedir(dir);
	response->addHeader("Content-Type", "text/html");
	response->addBody(body);
}

static void __read_file_content_to_do_response_(Client* client) {
	Context::Directives::const_iterator	directive, notFound;
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
	std::stringstream contentLengthStr;
	contentLengthStr << pathInfo.st_size;
	response->addHeader("Content-Length", contentLengthStr.str());
	response->buffer.append(CRLF);
}

Client* HTTP::getMethodHandler(Client *client)
{
	Context::Directives::const_iterator			directive, notFound;
	std::vector<std::string>::const_iterator	begin, end;
	Request										*request = client->getRequest();
	struct stat									pathInfo;

	notFound = request->location->getDirectives().end();
	// Check for redirection
	directive = request->location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != notFound) {
		throw std::make_pair(std::strtol(directive->second[0].c_str(), NULL, 10), directive->second[1]);
	}

	if (stat(request->fullPath.c_str(), &pathInfo) < 0) {
		throw 404;
	}
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode)) {
		if (request->path.back() != '/') {
			throw std::make_pair(301L, request->path + '/');
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
				return 0;
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
				directive = request->location->getDirectives().find(ROOT_DIRECTIVE);
				return HTTP::cgiExecutor(client, begin->c_str(), directive->second[0].c_str());
			}
	}

	__read_file_content_to_do_response_(client);
	return 0;
}
