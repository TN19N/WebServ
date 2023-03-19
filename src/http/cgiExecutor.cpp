// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/1/23 at 9:49 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "../../include/webserv/http.hpp"

static char *__set_env__key_and_value_(const char *key, const char *val)
{
	char 		*env, *save_env;
	size_t		len_key = 0, len_val = 0;
	
	while (key[len_key])
		++len_key;
	while (val[len_val])
		++len_val;
	env = new char [len_key + len_val + 1];
	save_env = env;
	while (*key)
		*env++ = *key++;
	while (*val)
		*env++ = *val++;
	*env = '\0';
	return save_env;
}

static void __run_cgi_script_(Client *client, int _read, int _write, const char *cgiPath)
{
	Request::Headers::const_iterator	header, notFound;
	char								**envs, **args;
	Request								*request = client->getRequest();

	// dup2 to change 'std in' and 'std out'
	if (dup2(STDIN_FILENO, _read) < 0 || dup2(STDOUT_FILENO, _write) < 0)
		exit(50);
	
	// fill args
	args = new char * [3];
	args[0] = __set_env__key_and_value_(cgiPath, "");
	args[1] = __set_env__key_and_value_(request->fullPath.c_str(), "");
	args[2] = 0;

	// fill env
	envs = new char * [10];
	notFound = request->headers.end();
	
	header = request->headers.find("Content-Length");
	if (header == notFound)
		envs[0] = __set_env__key_and_value_("CONTENT_LENGTH=", "-1");
	else
		envs[0] = __set_env__key_and_value_("CONTENT_LENGTH=", header->second.c_str());

	header = request->headers.find("Content-Type");
	if (header == notFound)
		envs[1] = __set_env__key_and_value_("CONTENT_TYPE=", DEFAULT_MIME_TYPE);
	else
		envs[1] = __set_env__key_and_value_("CONTENT_TYPE=", header->second.c_str());

	header = request->headers.find("Cookie");
	if (header == notFound)
		envs[2] = __set_env__key_and_value_("HTTP_COOKIE=", "");
	else
		envs[2] = __set_env__key_and_value_("HTTP_COOKIE=", header->second.c_str());

	envs[3] = __set_env__key_and_value_("GATEWAY_INTERFACE=", "CGI/1.1");
	envs[4] = __set_env__key_and_value_("QUERY_STRING=", request->query.c_str());
	envs[5] = __set_env__key_and_value_("REQUEST_METHOD=", request->method.c_str());
	envs[6] = __set_env__key_and_value_("SCRIPT_FILENAME=", request->fullPath.c_str());
	envs[7] = __set_env__key_and_value_("SERVER_SOFTWARE=", "webserv/1.0.0");
	envs[8] = __set_env__key_and_value_("SERVER_PROTOCOL=", "HTTP/1.1");

	// TODO: we can add more envs
	
	envs[9] = 0;
	// execute the cgi script
	if (execve(cgiPath, args, envs) < 0)
		exit(50);
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
