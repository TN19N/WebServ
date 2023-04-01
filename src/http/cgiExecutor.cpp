#include "webserv/http.hpp"

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

// create env from finding header value or use default value
static char *__set_new_env_(Request *req, const char *key, const char *env, const char *_default)
{
	Request::Headers::const_iterator	header;

	header = req->headers.find(key);
	if (header ==  req->headers.end())
		return __set_env__key_and_value_(env, _default);
	else
		return __set_env__key_and_value_(env, header->second.c_str());
}

static void __run_cgi_script_(Client *client, int _read, int _write, const char *cgiPath)
{
	char		**envs, **args;
	Request		*request = client->getRequest();

	// dup2 to change 'std in' and 'std out'
	if (dup2(_read, STDIN_FILENO) < 0 || dup2(_write, STDOUT_FILENO) < 0)
		exit(50);

	// fill args
	args = new char * [3];
	args[0] = __set_env__key_and_value_(cgiPath, "");
	args[1] = __set_env__key_and_value_(request->fullPath.c_str(), "");
	args[2] = 0;

	// fill env
	envs = new char * [11]; // to add more envs don't forget to allocate enough memory
	envs[0] = __set_env__key_and_value_("GATEWAY_INTERFACE=", "CGI/1.1");
	envs[1] = __set_env__key_and_value_("QUERY_STRING=", request->query.c_str());
	envs[2] = __set_env__key_and_value_("REQUEST_METHOD=", request->method.c_str());
	envs[3] = __set_env__key_and_value_("SCRIPT_FILENAME=", request->fullPath.c_str());
	envs[4] = __set_env__key_and_value_("SERVER_SOFTWARE=", "webserv/1.0.0");
	envs[5] = __set_env__key_and_value_("SERVER_PROTOCOL=", "HTTP/1.1");
	envs[6] = __set_env__key_and_value_("REDIRECT_STATUS=", "200");

	envs[7] = __set_new_env_(request, "Content-Length", "CONTENT_LENGTH=", "-1");
	envs[8] = __set_new_env_(request, "Content-Type", "CONTENT_TYPE=", DEFAULT_MIME_TYPE);
	envs[9] = __set_new_env_(request, "Cookie", "HTTP_COOKIE=", "");

	envs[10] = NULL;
	// execute the cgi script
	if (execve(cgiPath, args, envs) < 0) {
		exit(50);
	}
}

Client* HTTP::cgiExecutor(Client* client, const char *cgiPath, const char *rootDir)
{
	Client	*cgi;
	int		pid, _read[2], _write[2];
	
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

	if (close(_read[1]) < 0 || close(_write[0]) < 0) {
		throw 500;
	}

	if (fcntl(_read[0], F_SETFL, O_NONBLOCK) == -1) {
		throw 500;
	}

	if (fcntl(_write[1], F_SETFL, O_NONBLOCK) == -1) {
		throw 500;
	}

	cgi = new Client(_read[0], _write[1], pid, client);
	client->setClientToCgi(cgi);
	cgi->updateLastEvent();
	return cgi;
}
