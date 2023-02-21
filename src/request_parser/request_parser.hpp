// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 8:59 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#pragma once


#include <map>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

namespace ws
{
	struct _strcmp
	{
		bool operator()(const char *s1, const char *s2) const
		{
			while (*s1 && *s1 == *s2)
			{ ++s1; ++s2; }
			return *s1 < *s2;
		}
	};
	
	typedef std::map<std::string, std::string> Req;
}

char * request_parser(char *data, ws::Req &req);
