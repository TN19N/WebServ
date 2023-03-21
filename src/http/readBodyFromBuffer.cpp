// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/10/23 at 2:05 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/http.hpp"

static void __read_content_length_body_(Client* client) {
	Request*	request = client->getRequest();

	// contentLength always decrement until 0.
	request->body.append(client->getBuffer(), 0, request->contentLength);
	if (client->getBuffer().size() < request->contentLength) {
		request->contentLength -= client->getBuffer().size();
	} else {
		request->state = READY;
		request->contentLength = 0;
	}
	client->getBuffer().clear();
}

static bool __read_chunked_body_(Client* client) {
	Request			*request = client->getRequest();
	std::string&	Buffer = client->getBuffer();
	const char		*buffer = Buffer.c_str();

	if (request->contentLength == 0) {
		while (*buffer && (*buffer != '\r' || *(buffer+1) != '\n')) {
			++buffer;
		}
		if (*buffer == '\0') {
			return false;
		}
		Buffer[buffer - Buffer.c_str()] = '\0';
		buffer += 2; // for bypass '\r\n'
		request->contentLength = HTTP::parseContentLength(Buffer.c_str());
		if (request->contentLength == 0 && *buffer == '\r' && *(buffer + 1) == '\n') {
			request->state = READY;
			Buffer.clear();
			return false;
		}
		request->contentLength += 2; // this is for '\r\n'
		Buffer.erase(0,buffer - Buffer.c_str());
	} else {
		request->body.append(Buffer, 0, request->contentLength - 2); // -2 for '\r\n'
		if (Buffer.size() > request->contentLength)
		{
			Buffer.erase(0, request->contentLength);
			request->contentLength = 0;
		}
		else
		{
			request->contentLength -= Buffer.size();
			Buffer.clear();
		}
	}
	return Buffer.size() != 0;
}

void HTTP::readBodyFromBuffer(Client *client)
{
	if (client->getRequest()->isChunked) {
		while (__read_chunked_body_(client));
	} else {
		__read_content_length_body_(client);
	}
}
