// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/23/23 at 3:01 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "webserv/context.hpp"
# include "webserv/loadConfiguration.hpp"
# include "keep/http.hpp"

void HTTP::get_method_handler(const Context *location, Client *client)
{
	std::map<const std::string, std::vector<std::string> >::const_iterator directive, not_found;
	std::vector<std::string>::const_iterator begin, end;
	struct stat path_info;
	
	not_found = location->getDirectives().end();
	// Check for redirection
	directive = location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != not_found)
	{
		throw 301;
		// Redirect code directive->second[0] Location directive->second[1] ;
	}
	if (stat(client->getRequest()->path.c_str(), &path_info) < 0)
		throw 404;
	// Check is directory
	if (S_ISDIR(path_info.st_mode))
	{
		if (client->getRequest()->path.c_str()[client->getRequest()->path.size()-1] != '/')
			throw 301;
		
	}
	// Check for CGI
	directive = location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != not_found)
	{
		for (begin = directive->second.begin(), end = directive->second.end(); begin != end; ++begin)
			if (*begin == client->getRequest()->extension)
			{
				client->getRequest()->cgi = HTTP::cgi_executor(client->getRequest()->path, client->getRequest()->headers);
				return;
			}
	}
}
