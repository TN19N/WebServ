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

static char **__headers_to_args(const std::string &path, const IBase::Headers &headers)
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

int	HTTP::cgiExecutor(const std::string &path, const IBase::Headers &headers)
{
	int		pid;
	int		fds[2];
	char 	**args;
	
	if (pipe(fds) < 0)
		throw 500;
	pid = fork();
	if (pid < 0)
		throw 500;
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDIN_FILENO);
		args = __headers_to_args(path, headers);
		if (execve(path.c_str(), args, nullptr) < 0)
			exit(127);
	}
	close(fds[1]);
	return fds[0];
}
