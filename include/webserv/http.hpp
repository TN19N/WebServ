# ifndef HTTP_HPP
# define HTTP_HPP

# include <vector>
# include <poll.h>
# include <string>
#include <sys/stat.h>

# include "client.hpp"
# include "request.hpp"
# include "context.hpp"
#include "loadConfiguration.hpp"

# define CRLF "\r\n"

namespace HTTP {
    void    acceptConnection(int, std::vector<pollfd>&, std::vector<Client*>&);
    // request handler
    void    requestHandler(Client*, const Context*, std::vector<const Client*>&);
    Request* request_parser(const std::string&);
	void request_handler(Client*, const Context*);
    const Context* blockMatchAlgorithm(const Client*, const Context*);
	void get_method_handler(const Context*, Client*);
    // response handler
	bool sendResponse(const Client*, const int &, const std::string& = "", const std::map<std::string, std::string>& = std::map<std::string, std::string>());
	int cgi_executor(const std::string&, const std::map<std::string, std::string> &);
    // status code
    const std::string   getStatusCodeMessage(int);
    // cgi handler
    Client* runCgi(const Context*, const Client*);
    // tools
    const std::string   getHttpDate();
}
# endif