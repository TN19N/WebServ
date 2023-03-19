// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/1/23 at 9:49 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "../../include/webserv/http.hpp"

static char *__duplicate_string(const std::string &string)
{
	const char *str = string.c_str();
	char *result, *save_result;
	
	result = new char [string.size() + 1];
	save_result = result;
	while (*str)
		*result++ = *str++;
	*result = '\0';
	return save_result;
}

static char **__set_args_(const std::string &path, const IBase::Headers &headers)
{
	IBase::Headers::const_iterator begin, end;
	char	**args;
	
	begin = headers.begin();
	end = headers.end();
	args = new char *[headers.size() + 2];
	args[0] = __duplicate_string(path);
	for (size_t i = 1; begin != end; ++begin, ++i)
		args[i] = __duplicate_string(begin->first + ": " + begin->second);
	args[headers.size() + 1] = nullptr;
	return args;
}

Client* HTTP::cgiExecutor(Client* client)
{
	Client	*cgi;
	int		pid;
	int		_read[2];
	int		_write[2];
	char 	**args;
	
	if (pipe(_read) < 0 || pipe(_write))
		throw 500;
	pid = fork();
	if (pid < 0)
		throw 500;
	cgi = new Client(_read[0], _write[1], pid, client);
	
	
	close(_read[1]);
	close(_write[0]);
	return cgi;
}
