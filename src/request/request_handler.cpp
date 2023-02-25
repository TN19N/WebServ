// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/22/23 at 4:20 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request.hpp"

static const Context* __get_match_server_context_(std::vector<const Context*> &servers, std::string &host)
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		if (servers[i]->getDirectives(NAME_DIRECTIVE) == host)
			return servers[i];
	}
	return servers[0];
}

static const Context* __get_match_location_context_(std::vector<const Context*> &locations, std::string &path)
{

}

void request_handler(char *buffer, Request &request, std::vector<const Context*> &servers)
{
	request_parser(buffer, request);
	const Context* server = __get_match_server_context_(servers, request.headers.find("Host")->second);
	
	if (request.method == "GET")
		get_method_handler(server, request);
	
	
}
