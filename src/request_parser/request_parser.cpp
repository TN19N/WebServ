// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 1:37 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request_parser.hpp"

static bool _is_space(char c)
{
	if (c == ' ' || c == '\t')
		return true;
	return false;
}
//static void _skipp_spaces(char * &data)
//{
//	while (*data == ' ' || *data == '\t')
//		++data;
//	if (*data == '\0')
//		throw 400;
//}

static char * _get_req_method(char * &data)
{
	char *save_data = data;
	
	 if (strcmp(data, "GET") == ' ')
		 data += 3;
	 else if (strcmp(data, "POST") == ' ')
		 data += 4;
	 else if (strcmp(data, "DELETE") == ' ')
		 data += 6;
	 else
		 throw 400;
	 
	 *data = '\0';
	 ++data;
	 return save_data ;
}

static char * _get_filepath(char * & data)
{
	char *save_data = data;
	
	while (*data && *data != ' ')
		++data;
	if (*data == '\0')
		throw 400;
	*data = '\0';
	++data;
	return save_data;
}

static void _check_protocol(char * &data)
{
	if (strcmp(data, "HTTP/1.1") == '\n')
		data += 9;
	else
		throw 400;
}

static char *_get_header_key(char * &data)
{
	char *save_data = data;
	
	while (*data && *data != ':' && !_is_space(*data))
		++data;
	if (*data == '\0' || _is_space(*data))
		throw 400;
	*data = '\0';
	++data;
	if (*data != ' ')
		throw 400;
	++data;
	if (_is_space(*data))
		throw 400;
	return save_data;
}

static char *_get_header_value(char * &data)
{
	char *save_data = data;
	
	while (*data && *data != '\n' && !_is_space(*data))
		++data;
	if (*data == '\0' || _is_space(*data))
		throw 400;
	*data = '\0';
	++data;
	return save_data;
}

char * request_parser(char *data, ws::Req &req)
{
	char *key;
	
	req["method"] = _get_req_method(data) ;
	req["filepath"] = _get_filepath(data);
	_check_protocol(data);
	do
	{
		if (*data == '\0')
			throw 400;
		key = data;
		req[_get_header_key(data)];
		req[key] = _get_header_value(data);
		
	}
	while (*data != '\n');
	return data + 1;
}
