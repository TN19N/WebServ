
# include <unistd.h>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"

/*
static bool __send_response_to_client_(Client* client) {
    int res;

    if (client->isCgi()) {
        Request* request = client->getRequest();

        if ( ! request->body.empty()) {
            if ((res = write(client->getFdOf(WRITE_END), request->body.c_str(), std::min(BUFFER_SIZE, request->body.length()))) == -1) {
                throw 500;
            }
            request->body.erase(0, res);
        }

        if (request->body.empty()) {
            return true;
        }
    } else {
        Response* response = client->getResponse();

        if ( ! response->buffer.empty()) {
            if ((res = send(client->getFdOf(WRITE_END), response->buffer.c_str(), std::min(BUFFER_SIZE, response->buffer.length()), 0)) == -1) {
                throw 500;
            }
            response->buffer.erase(0, res);
        }

        if (response->buffer.empty()) {
            if (response->keepAlive) {
				client->switchState();
            } else {
                return true;
			}
        }
    }

    return false;
}
*/

bool HTTP::responseHandler(Client *client)
{
	Response	*response = client->getResponse();
	char		buffer[BUFFER_SIZE];
	ssize_t		readSize;
	bool		ret = false;

	switch (client->getState())
	{
		case DOWNLOADING_FILE:
			readSize = read(response->download_file_fd, buffer, BUFFER_SIZE);
			if (readSize < 0)
				throw 500;
			response->buffer.append(buffer, readSize);
			if (readSize < BUFFER_SIZE)
				ret = true;
			break;
	}
	return ret && HTTP::sendResponseBufferToClient(client);
}
