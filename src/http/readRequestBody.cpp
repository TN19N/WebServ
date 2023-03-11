// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/10/23 at 2:05 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static int __parse_content_length_(const char *str)
{
	const char 	*checker = str;
	long int	res = 0;
	
	if (*checker == '\0')
		throw 400;
	while (*checker)
	{
		if (*checker < '0' || *checker > '9')
			throw 400;
		++checker;
	}
	while (*str && res < INT_MAX)
	{
		res = res * 10 + *str - '0' ;
		++str;
	}
	if (*str == '\0')
		return res;
	throw 400;
}

static void __read_content_length_body_(Client* client, size_t size)
{
	client->getRequest()->body.append(client->getBuffer(), 0, size) ;
	client->getBuffer().clear() ;
	if (client->getRequest()->body.size() == client->getRequest()->contentLength)
		client->getRequest()->state = REQUEST_READY ;
}

static bool __read_chunked_body_(Client* client)
{
	const char *buffer = client->getBuffer().c_str();

	if (client->getRequest()->contentLength == 0)
	{
		while (*buffer && (*buffer != '\r' || *(buffer+1) != '\n'))
			++buffer;
		if (*buffer == '\0')
			return false;
		client->getBuffer()[buffer - client->getBuffer().c_str()] = '\0';
		buffer += 2;
		client->getRequest()->contentLength = __parse_content_length_(client->getBuffer().c_str());
		if (client->getRequest()->contentLength == 0 && *buffer == '\r' && *(buffer+1) == '\n')
		{
			client->getRequest()->state = REQUEST_READY;
			return false;
		}
		client->getRequest()->contentLength += 2; // this is for '\r\n'
		client->getBuffer().erase(0,buffer - client->getBuffer().c_str());
	}
	else
	{
		client->getRequest()->body.append(client->getBuffer(), 0,
							client->getRequest()->contentLength - 2); // -2 for '\r\n'
		if (client->getBuffer().size() > client->getRequest()->contentLength)
		{
			client->getBuffer().erase(0, client->getRequest()->contentLength);
			client->getRequest()->contentLength = 0;
		}
		else
		{
			client->getRequest()->contentLength -= client->getBuffer().size();
			client->getBuffer().clear();
		}
	}
	return client->getBuffer().size() != 0;
}

void HTTP::readRequestBodyFromBuffer(Client* client)
{
	if (client->getRequest()->isChunked)
		while (__read_chunked_body_(client)) ;
	else
		__read_content_length_body_(client, client->getRequest()->contentLength - client->getRequest()->body.size());
}
