// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/1/23 at 9:49 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "../../include/webserv/http.hpp"
/*
static char *__new_arg__key_and_value_(const char *key, const std::string &value)
{
	const char	*val = value.c_str();
	char 		*result, *save_result;
	size_t		len = 0;
	
	while (key[len])
		++len;
	result = new char [len + value.size() + 1];
	save_result = result;
	while (*key)
		*result++ = *key++;
	while (*val)
		*result++ = *val++;
	*result = '\0';
	return save_result;
}

static char **__set_args_()
{
	char	**args;
	
	
	return args;
}
 */

static void __run_cgi_script_(Client *client, int _read, int _write, const char *cgiPath)
{


}

Client* HTTP::cgiExecutor(Client* client, const char *cgiPath, const char *rootDir)
{
	int	pid, _read[2], _write[2];
	
	if (pipe(_read) < 0 || pipe(_write))
		throw 500;
	pid = fork();
	if (pid < 0)
		throw 500;
	if (pid == 0)
	{
		if (close(_read[0]) < 0 || close(_write[1]) < 0)
			exit(50);
		if (chdir(rootDir) < 0)
			exit(50);
		__run_cgi_script_(client, _write[0], _read[1], cgiPath);
	}

	if (close(_read[1]) < 0 || close(_write[0]) < 0)
		throw 500;
	return (new Client(_read[0], _write[1], pid, client));
}
