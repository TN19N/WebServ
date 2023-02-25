// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 5:41 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request/request.hpp"

int main()
{
	Request	req;
	char	*data;
	int		fd;

	data = new char[13371];
	fd = open("./http_request", O_RDONLY);
	data[read(fd, data, 13370)] = '\0';
	try
	{
		request_handler(data, req);
		for (Headers::iterator b = req.headers.begin(), e = req.headers.end(); b != e; ++b)
			std::cout << b->first << ": " << b->second << '\n' ;
	}
	catch (int error)
	{
		std::cerr << "Error: " << error << '\n' ;
	}
	std::cout << "\nHost: " << req.headers.find("Host")->second << '\n';
}
