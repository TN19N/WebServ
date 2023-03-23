# include <unistd.h>
# include <iostream> // TODO: remove this
# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"

static size_t __get_current_time_on_milli_second_()
{
	timeval	tv;

	gettimeofday(&tv, 0);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static bool __send_client_body_to_cgi_(Client *client) {
	Request			*request = client->getCgiToClient()->getRequest();
	ssize_t			writeSize;
	std::string&	buffer = client->getBuffer();
	
	if (buffer.size() < BUFFER_SIZE) {
		writeSize = std::min((size_t)BUFFER_SIZE, request->body.size());
		buffer.append(request->body, 0, writeSize);
		request->body.erase(0, writeSize);
	}
	writeSize = write(client->getFdOf(WRITE_END), buffer.c_str(), buffer.size());
	if (writeSize < 0) {
		throw 500;
	}
	buffer.erase(0, writeSize);
	return buffer.empty() && request->state == READY && request->body.empty();
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
	Client	*clientOfCgi = client->getCgiToClient();

	if (client->isCgi()) {
		if (__send_client_body_to_cgi_(client)) {
			clientOfCgi->setCgiLastSeen(__get_current_time_on_milli_second_());
			clientOfCgi->switchState();
			client->switchState();
		}
	} else {
		if (client->getClientToCgi()) {
			if (__get_current_time_on_milli_second_() > client->getCgiLastSeen() + CGI_TIMEOUT) {
				throw 504;
			}
			return true;
		}
		return __send_response_to_client_(client);
	}
		
	return true;
}
