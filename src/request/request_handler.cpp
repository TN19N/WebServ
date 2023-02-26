// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/22/23 at 4:20 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "webserv/request.hpp"
# include "webserv/context.hpp"
# include "webserv/loadConfiguration.hpp"
# include "webserv/http.hpp"

static int __strcmp_(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{ ++s1; ++s2; }
	return *s1 - *s2;
}

static const Context* __get_match_server_context_(std::vector<const Context*> &servers, std::string &host)
{
	std::map<const std::string, std::vector<std::string> >::const_iterator d_begin, d_end;
	std::vector<std::string>::const_iterator begin, end;
	
	for (size_t i = 0; i < servers.size(); ++i)
	{
		d_begin = servers[i]->getDirectives().find(NAME_DIRECTIVE);
		d_end = servers[i]->getDirectives().end();
		if (d_begin != d_end)
		{
			for (begin = d_begin->second.begin(), end = d_end->second.end(); begin != end; ++begin)
				if (*begin == host)
					return servers[i];
		}
	}
	return servers[0];
}

static const Context* __get_match_location_context_(const std::vector<Context*> &locations, std::string &path)
{
	int		character;
	std::map<std::string, const Context*, std::greater<std::string> > locate;
	std::map<std::string, const Context*, std::greater<std::string> >::const_iterator begin, end;
	
	if (locations.empty())
		throw "no location found";
	for (size_t i = 0; i < locations.size(); ++i)
		locate.insert(std::make_pair(locations[i]->getArgs()[1], locations[i]));
	
	for (begin = locate.begin(), end = locate.end(); begin != end; ++begin)
	{
		character = __strcmp_(path.c_str(), begin->first.c_str());
		if (character == 0)
		{
			// Return 301 move to path/to'/' (slash at the end)
			throw 301;
		}
		if (character == '/')
			return begin->second;
	}
	throw "no match any location: Forbidden";
}

void HTTP::request_handler(Request &request, std::vector<const Context*> &servers)
{
	const Context* server = __get_match_server_context_(servers, request.headers.find("Host")->second);
	const Context* location = __get_match_location_context_(server->getChildren(), request.path);

	request.path = location->getDirectives().find(ROOT_DIRECTIVE)->second + request.path;
	if (request.method == "GET")
		get_method_handler(location, request);
	
}
