// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 12/03/2023 at 18:22.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

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
	
}
