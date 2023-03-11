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

static void __do_response_with_dir_content_(Client* client)
{
	Response		*response;
	std::string		body;
	std::string		&path = client->getRequest()->path;
	DIR* 			dir;
	struct dirent*	ent;
	
	response = new Response(200, false);
	client->setResponse(response);
	
	if ((dir = opendir(client->getRequest()->fullPath.c_str())) == nullptr)
		throw 403;
	while ((ent = readdir(dir)))
	{
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
		body.append(1, '\n');
	}
	closedir(dir);
	response->addBody(body);
	client->switchState();
//	std::cerr << response->buffer << '\n' ;
}

void HTTP::getMethodHandler(Client *client)
{
	std::map<std::string, std::vector<std::string> >::const_iterator	directive, notFound;
	std::vector<std::string>::const_iterator							begin, end;
	
	const Context* const	location = client->getLocation();
	Request					*request = client->getRequest();
	struct stat				pathInfo;
	
	notFound = location->getDirectives().end();
	// Check for redirection
	directive = location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != notFound)
		throw std::make_pair(std::stoi(directive->second[0]), directive->second[1]) ;
	if (stat(request->fullPath.c_str(), &pathInfo) < 0)
		throw 404;
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode))
	{
		if (request->path.c_str()[request->path.size()-1] != '/')
			throw std::make_pair(301, request->path + '/');
		else if (location->getDirectives().find(AUTOINDEX_DIRECTIVE)->second[0] == DEFAULT_AUTOINDEX)
			throw 403;
		else
//			throw 1024;
			__do_response_with_dir_content_(client);
	}
	// Check for CGI
	directive = location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != notFound)
	{
		for (begin = directive->second.begin(), end = directive->second.end(); begin != end; ++begin)
			if (*begin == request->extension)
			{
				throw 1337;
			}
	}
	throw 1337;
}
