// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 5:41 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request_parser.hpp"

int main(int ac, const char *av[])
{
	ws::Req		req;
	char		*data;
	int			fd;
	
	if (ac != 2)
	{
		std::cerr << "Bad Arguments\n" ;
		return 1;
	}
	data = new char[13371];
	fd = open("./http_request", O_RDONLY);
	data[read(fd, data, 13370)] = '\0';
	try
	{
			request_parser(data, req);
			for (ws::Req::iterator b = req.begin(), e = req.end(); b != e; ++b)
				std::cout << b->first << ": " << b->second << '\n' ;
	}
	catch (int error)
	{
		std::cerr << "Error: " << error << '\n' ;
	}
	
	ws::Req::iterator header = req.find(av[1]);
	if (header == req.end())
		std::cout << "Not Found\n" ;
	else
		std::cout << header->first << ": " << header->second << '\n' ;
}
