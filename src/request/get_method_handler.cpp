// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/23/23 at 3:01 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request.hpp"

void get_method_handler(const Context *location, Request &request)
{
	std::map<const std::string, std::vector<std::string> >::const_iterator directive, not_found;
	std::vector<std::string>::const_iterator begin, end;
	std::ifstream file;
	
	
	not_found = location->getDirectives().end();
	directive = location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != not_found)
	{
		// Redirect code directive->second[0] Location directive->second[1] ;
	}
	directive = location->getDirectives().find(CGI_DIRECTIVE);
	if (directive != not_found)
	{
		for (begin = directive->second.begin(), end = directive->second.end(); begin != end; ++begin)
			if (*begin == request.extension)
			{
				// Go to CGI
				return;
			}
	}
	
}
