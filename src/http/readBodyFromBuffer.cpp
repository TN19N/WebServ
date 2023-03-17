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
	const char *buffer = client->getBuffer().c_str();

	if (client->getRequest()->contentLength == 0) {
		while (*buffer && (*buffer != '\r' || *(buffer+1) != '\n')) {
			++buffer;
		}
		if (*buffer == '\0') {
			return false;
		}
		client->getBuffer()[buffer - client->getBuffer().c_str()] = '\0';
		buffer += 2;
		client->getRequest()->contentLength = HTTP::parseContentLength(client->getBuffer().c_str());
		if (client->getRequest()->contentLength == 0 && *buffer == '\r' && *(buffer+1) == '\n') {
			client->getRequest()->state = READY;
			return false;
		}
		client->getRequest()->contentLength += 2; // this is for '\r\n'
		client->getBuffer().erase(0,buffer - client->getBuffer().c_str());
	} else {
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

void HTTP::readBodyFromBuffer(Client *client)
{
	if (client->getRequest()->isChunked) {
		while (__read_chunked_body_(client));
	} else {
		__read_content_length_body_(client);
	}
}
