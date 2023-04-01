# include <unistd.h>
# include "webserv/response.hpp"
# include "webserv/http.hpp"

static bool __send_client_body_to_cgi_(Client *cgi) {
	Request			*request = cgi->getCgiToClient()->getRequest();
	ssize_t			writeSize;

	writeSize = std::min((size_t)BUFFER_SIZE, request->body.size());
	writeSize = write(cgi->getFdOf(WRITE_END), request->body.c_str(), writeSize);
	if (writeSize < 0) {
		throw 500;
	}
	request->body.erase(0, writeSize);
	return request->state == READY && request->body.empty();
}

static bool __send_response_buffer_to_client_(Client *client) {
	ssize_t			writeSize;
	std::string&	buffer = client->getResponse()->buffer;
	
	writeSize = send(client->getFdOf(WRITE_END), buffer.c_str(), buffer.size(), 0);
	if (writeSize < 0) {
		throw 500;
	}
	buffer.erase(0, writeSize);
	return buffer.empty();
}

static bool __send_response_to_client_(Client *client)
{
	Response	*response = client->getResponse();
	char		buffer[BUFFER_SIZE];
	ssize_t		readSize = -1;
	bool        keepAlive = response->keepAlive;
	
	if (response->download_file_fd != -1) {
		if (response->buffer.size() < BUFFER_SIZE * 5) {
			if ((readSize = read(response->download_file_fd, buffer, BUFFER_SIZE)) < 0) {
				throw 500;
			}
			response->buffer.append(buffer, readSize);
		}
		
		__send_response_buffer_to_client_(client);
		
		if (readSize == 0) {
			close(response->download_file_fd);
			response->download_file_fd = -1;
		}
	} else if (__send_response_buffer_to_client_(client)) {
		client->switchState();
		return keepAlive;
	}
	return true;
}

bool HTTP::responseHandler(Client *client)
{
	if (client->isCgi()) {
		client->updateLastEvent();
		if (__send_client_body_to_cgi_(client)) {
			client->switchState();
		}
	} else {
		if (client->getClientToCgi()) {
			return true;
		}
		return __send_response_to_client_(client);
	}

	return true;
}
