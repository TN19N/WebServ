
# include <unistd.h>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"
# include "../../include/webserv/client.hpp"

# define SEND_BUFFER_SIZE 4096UL

const bool HTTP::sendResponse(Client* client) {
    int res = 0;

    if (client->isCgi()) {
        Request* request = client->getRequest();

        if (request->body.empty() == false) {
            if ((res = write(client->getFdOf(WRITE_END), request->body.c_str(), std::min(SEND_BUFFER_SIZE, request->body.length()))) == -1) {
                throw 500;
            }
            request->body.erase(0, res);
        }

        if (request->body.empty() == true) {
            return true;
        }
    } else {
        Response* response = client->getResponse();

        if (response->buffer.empty() == false) {
            if ((res = send(client->getFdOf(WRITE_END), response->buffer.c_str(), std::min(SEND_BUFFER_SIZE, response->buffer.length()), 0)) == -1) {
                throw 500;
            }
            response->buffer.erase(0, res);
        }

        if (response->buffer.empty() == true) {
            if (response->keepAlive == false) {
                return true;
            }
        }
    }

    return false;
}
