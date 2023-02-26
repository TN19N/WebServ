// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 8:59 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <fstream>
#include <ios>

#include "../../include/webserv/request.hpp"
#include "../../include/webserv/context.hpp"
#include "../../include/webserv/loadConfiguration.hpp"

struct _strcmp
{
	bool operator()(const char *s1, const char *s2) const
	{
		while (*s1 && *s1 == *s2)
		{ ++s1; ++s2; }
		return *s1 < *s2;
	}
};

void request_handler(char *buffer, Request &request);
void request_parser(char *buffer, Request &request);
void get_method_handler(const Context *server, Request &request);
