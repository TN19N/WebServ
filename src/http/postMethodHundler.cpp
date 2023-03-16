// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 12/03/2023 at 18:22.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static void __create_file_to_upload_content_(Client *client, const char *filepath)
{
	Request	*request = client->getRequest();
	
	if (access(filepath, F_OK) == 0)
		throw 403;
	request->upload_file_fd = open(filepath, O_WRONLY | O_CREAT);
	if (request->upload_file_fd < 0)
		throw 403;
	
	if (write(request->upload_file_fd, request->body.c_str(), request->body.size()) < 0)
		throw 500;
	request->body.clear();
	if (request->state == BODY_READY)
	{
		client->setResponse(new Response(200, false));
		client->switchState();
	}
	else
		client->setState(UPLOADING_FILE);
}

void HTTP::postMethodHandler(Client *client)
{
	std::map<std::string, std::vector<std::string> >::const_iterator	directive, notFound;
	std::vector<std::string>::const_iterator							begin, end;
	
	Request					*request = client->getRequest();
	struct stat				pathInfo;
	
	notFound = request->location->getDirectives().end();
	// Check for redirection
	directive = request->location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != notFound)
		throw std::make_pair(std::stoi(directive->second[0]), directive->second[1]) ;
	// check for file upload
	directive = request->location->getDirectives().find(UPLOAD_DIRECTIVE);
	if (directive != notFound)
	{
		// TODO: logic of filepath name here <<<-------
		__create_file_to_upload_content_(client, (directive->second[0] + "test").c_str());
		return;
	}
	// check is exist whatever file or directory
	if (stat(request->fullPath.c_str(), &pathInfo) < 0)
		throw 404;
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode))
	{
		if (request->path.c_str()[request->path.size()-1] != '/')
			throw std::make_pair(301, request->path + '/');
		else
		{
			directive = request->location->getDirectives().find(INDEX_DIRECTIVE);
			for (begin = directive->second.begin(), end = directive->second.end(); begin != end; ++begin)
			{
				if (access((request->fullPath + *begin).c_str(), F_OK) == 0)
				{
					request->fullPath.append(*begin);
					request->path.append(*begin);
					request->extension = HTTP::getExtensionFromPath(request->path.c_str());
					break;
				}
			}
			if (begin == end)
				throw 403;
		}
	}
	// Check for CGI
	directive = request->location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != notFound)
	{
		for (begin = directive->second.begin(), ++begin, end = directive->second.end(); begin < end; ++begin, ++begin)
			if (*begin == request->extension)
			{
				--begin;
				throw 1337;
			}
	}
	// no CGI just a simple file
	throw 403;
}
