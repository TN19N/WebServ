// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 1:37 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/request.hpp"
# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"

static bool __is_space_(char c)
{
	if (c == ' ' || c == '\t')
		return true;
	return false;
}
static void __skipp_spaces_(const char * &buffer)
{
	while (*buffer == ' ' || *buffer == '\t')
		++buffer;
	if (*buffer == '\0')
		throw 400;
}

static const char *__get_request_method_(const char * &buffer, std::string &base)
{
	const char *save_buffer = buffer;
	
	if (HTTP::strcmp(buffer, "GET") == ' ')
		buffer += 3;
	else if (HTTP::strcmp(buffer, "POST") == ' ')
		buffer += 4;
	else if (HTTP::strcmp(buffer, "DELETE") == ' ')
		buffer += 6;
	else
		throw 501;

	base[buffer - base.c_str()] = '\0';
	++buffer;
	while (*buffer == ' ')
		++buffer;
	return save_buffer ;
}

static const char *__get_requested_path_(const char * &buffer, std::string &base)
{
	const char *save_buffer = buffer;
	
	while (*buffer && *buffer != ' ')
		++buffer;
	if (*buffer == '\0')
		throw 400;
	base[buffer - base.c_str()] = '\0';
	++buffer;
	while (*buffer == ' ')
		++buffer;
	return save_buffer;
}

static const char *__get_query_from_path_(const char *path, std::string &base)
{
	while (*path && *path != '?')
		++path;
	if (*path == '\0')
		return path;
	base[path - base.c_str()] = '\0';
	return path + 1;
}

static void __check_request_protocol_(const char * &buffer)
{
	if (HTTP::strcmp(buffer, "HTTP/1.1") == '\r')
		buffer += 9;
	else if (HTTP::strcmp(buffer, "HTTP/1.1") == ' ')
	{
		buffer += 9;
		while (*buffer == ' ')
			++buffer;
		if (*buffer != '\r')
			throw 400;
		++buffer;
	}
	else
		throw 505;
	if (*buffer != '\n')
		throw 400;
	++buffer;
}

static const char *__get_header_key_(const char * &buffer, std::string &base)
{
	const char *save_buffer = buffer;
	
	while (*buffer && *buffer != ':' && !__is_space_(*buffer) && *buffer != '\r')
		++buffer;
	if (*buffer == '\0' || __is_space_(*buffer) || *buffer == '\r')
		throw 400;
	base[buffer - base.c_str()] = '\0';
	++buffer;
	return save_buffer;
}

static const char *__get_header_value_(const char * &buffer, std::string &base)
{
	const char *save_buffer, *end_of_value;

	__skipp_spaces_(buffer);
	save_buffer = buffer;
	end_of_value = buffer;
	while (*buffer && *buffer != '\r')
	{
		if (__is_space_(*buffer))
			__skipp_spaces_(buffer);
		else
		{
			++buffer;
			end_of_value = buffer;
		}
	}
	if (*buffer == '\0' || *(buffer+1) != '\n')
		throw 400;
	if (end_of_value == save_buffer)
		throw 400;
	base[end_of_value - base.c_str()] = '\0';
	buffer += 2;
	return save_buffer;
}

static void __fill_request_and_check_basic_bad_errors_(IBase *base, bool isCgi) {
	std::string::size_type		pos;
	IBase::Headers::iterator	header, notFound;

	notFound = base->headers.end();
	if ( ! isCgi) {
		header = base->headers.find("Host");
		if (header == notFound) {
			throw 400;
		}
		pos = header->second.rfind(':');
		if (pos != std::string::npos) {
			header->second = header->second.substr(0, pos);
		}
	}

	header = base->headers.find("Transfer-Encoding");
	if (header != notFound) {
		if (header->second != "chunked") {
			throw 501;
		}
		base->isChunked = true;
	} else {
		header =  base->headers.find("Content-Length");
		if (header != notFound) {
			base->contentLength = HTTP::parseContentLength(header->second.c_str());
		}
	}
}

static void __headers_parser_(const char * &buffer, std::string &Buffer, IBase *base, bool isCgi) {
	const char	*key;
	IBase::Headers				&headers = base->headers;
	std::vector<std::string>	&setCookie = base->setCookie;

	while (*buffer && *buffer != '\r')
	{
		key = buffer;
		if (headers[__get_header_key_(buffer, Buffer)].empty())
			headers[key] = __get_header_value_(buffer, Buffer);
		else if (HTTP::strcmp(key, "Host") == 0)
			throw 400;
		else if (HTTP::strcmp(key, "Set-Cookie") == 0 && isCgi)
			setCookie.push_back(__get_header_value_(buffer, Buffer));
		else
			headers[key] = __get_header_value_(buffer, Buffer);
	}
	if (*buffer == '\0' || *(buffer + 1) != '\n')
		throw 400;
	buffer += 2;
}

// parse whatever client request or cgi response
IBase* HTTP::baseParser(Client *client)
{
	Request		*request 	= 0;
	Response	*response 	= 0;
	const char	*path, *buffer = client->getBuffer().c_str();
	
	try
	{
		if (client->isCgi()) {
			response = new Response();
			__headers_parser_(buffer, client->getBuffer(), response, client->isCgi());
			__fill_request_and_check_basic_bad_errors_(response, client->isCgi());
			client->getBuffer().erase(0,buffer - client->getBuffer().c_str()); // here erase buffer until body begin
		} else {
			request = new Request();
			request->method = __get_request_method_(buffer, client->getBuffer()) ;
			path = __get_requested_path_(buffer, client->getBuffer());
			if (path[0] != '/')
				throw 400;
			request->query = HTTP::urlDecoding(__get_query_from_path_(path, client->getBuffer()));
			request->path = HTTP::urlDecoding(path);
			if (PATH_MAX < request->path.size())
				throw 414;
			request->extension = HTTP::getExtensionFromPath(path);
			__check_request_protocol_(buffer);
			if (*buffer == '\r')
				throw 400;
			__headers_parser_(buffer, client->getBuffer(), request, client->isCgi());
			__fill_request_and_check_basic_bad_errors_(request, client->isCgi());
			client->getBuffer().erase(0,buffer - client->getBuffer().c_str()); // here erase buffer until body begin
			if (request->method != "POST")
			{
				client->getBuffer().clear();
				request->state = READY;
			}
		}
	}
	catch (const int error)
	{
		delete request;
		delete response;
		throw error;
	}
	catch (const std::exception &e)
	{
		delete request;
		delete response;
		throw e;
	}
	if (request != 0)
		return request;
	return response ;
}
