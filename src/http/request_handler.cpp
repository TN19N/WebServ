// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/27/23 at 2:51 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/http.hpp"

void HTTP::request_handler(Client* client, const Context* configuration)
{
	const Context* location = HTTP::blockMatchAlgorithm(client, configuration);
	std::string path;
	
	path = location->getDirectives().find(ROOT_DIRECTIVE)->second[0] + client->getRequest()->path;
	client->getRequest()->path = path;
	try
	{
		if (client->getRequest()->method == "GET")
			HTTP::get_method_handler(location, client);
	}
	catch (const char *error)
	{
		std::cerr << "Error (const char*): " << error << '\n' ;
		exit (2);
	}
	catch (int error)
	{
		std::cerr << "Error (int): " << error << '\n' ;
		exit (2);
	}
	catch (...)
	{
		std::cerr << "Error (unknown)\n" ;
		exit (2);
	}
}
