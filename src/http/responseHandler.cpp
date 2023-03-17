# include <unistd.h>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"

bool HTTP::responseHandler(Client *client) {
	Response	*response = client->getResponse();
	char		buffer[BUFFER_SIZE];
	ssize_t		readSize;
	bool		ret = false;
    bool        keepAlive = response->keepAlive;

    if (response->download_file_fd != -1) {
        if ((readSize = read(response->download_file_fd, buffer, BUFFER_SIZE)) < 0) {
            throw 500;
        }

		response->buffer.append(buffer, readSize);
        HTTP::sendResponseBufferToClient(client);

        if (readSize == 0) {
            close(response->download_file_fd);
            response->download_file_fd = -1;
        }
    } else if (HTTP::sendResponseBufferToClient(client)) {
        client->switchState();
        return keepAlive;
    }

    return true;
}