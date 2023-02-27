// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/27/23 at 2:51 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#pragma once

#include "webserv/http.hpp"

void HTTP::request_handler(const Context *location, const Client *client)
{
	if (client->getRequest()->method == "GET")
		HTTP::get_method_handler(location, client);
}
