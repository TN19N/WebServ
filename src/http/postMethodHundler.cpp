# include <iostream>
# include "webserv/context.hpp"
# include "webserv/http.hpp"

static void __create_file_to_upload_content_(Client *client, const char *filepath) {
	Request	*request = client->getRequest();

	if (access(filepath, F_OK) == 0) {
		throw 403;
	}

	request->upload_file_fd = open(filepath, O_WRONLY | O_CREAT);
	if (request->upload_file_fd < 0) {
		throw 403;
	}
	request->upload_file_name = filepath;
}

Client* HTTP::postMethodHandler(Client *client) {
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
	// check for file upload
	directive = request->location->getDirectives().find(UPLOAD_DIRECTIVE);
	if (directive != notFound)
	{
		std::string fileName;
		try {
			fileName = directive->second[0] + "/" + request->headers.at("FILE-NAME");
		} catch (std::out_of_range &e) {
			throw 400;
		}

		if (request->headers.at("FILE-NAME").find('/') != std::string::npos) {
			throw 403;
		}
		__create_file_to_upload_content_(client, fileName.c_str());
		return 0;
	}

	if (stat(request->fullPath.c_str(), &pathInfo) < 0) {
		throw 404;
	}
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode))
	{
		if (request->path.c_str()[request->path.size() - 1] != '/') {
			throw std::make_pair(301L, request->path + '/');
		} else {
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
			if (begin == end) {
				throw 403;
			}
		}
	}

	directive = request->location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != notFound)
	{
		for (begin = directive->second.begin(), ++begin, end = directive->second.end(); begin < end; ++begin, ++begin)
			if (*begin == request->extension)
			{
				--begin;
				directive = request->location->getDirectives().find(ROOT_DIRECTIVE);
				return HTTP::cgiExecutor(client, begin->c_str(), directive->second[0].c_str());
			}
	}

	throw 403;
}
