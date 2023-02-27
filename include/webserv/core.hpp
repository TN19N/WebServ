# ifndef CORE_HPP
# define CORE_HPP

# define MAIN_CONTEXT       "main"
# define HTTP_CONTEXT       "http"
# define SERVER_CONTEXT     "server"
# define LOCATION_CONTEXT   "location"
# define TYPES_CONTEXT      "types"

# define HTTP_CONTEXT_ARGS(num)     (num == 0)
# define SERVER_CONTEXT_ARGS(num)   (num == 0)
# define LOCATION_CONTEXT_ARGS(num) (num == 1)
# define TYPES_CONTEXT_ARGS(num)    (num == 0)

# define HTTP_CONTEXT_PARENT(parent)     (parent == MAIN_CONTEXT)
# define SERVER_CONTEXT_PARENT(parent)   (parent == HTTP_CONTEXT)
# define LOCATION_CONTEXT_PARENT(parent) (parent == SERVER_CONTEXT)
# define TYPES_CONTEXT_PARENT(parent)    (parent == HTTP_CONTEXT)

# define PORT_DIRECTIVE         "port"
# define HOST_DIRECTIVE         "host"
# define NAME_DIRECTIVE         "name"
# define ROOT_DIRECTIVE         "root"
# define INDEX_DIRECTIVE        "index"
# define ERROR_DIRECTIVE        "error_page"
# define SIZE_DIRECTIVE         "client_max_body_size"
# define METHOD_DIRECTIVE       "allowed_method"
# define AUTOINDEX_DIRECTIVE    "autoindex"
# define CGI_DIRECTIVE          "cgi"
# define UPLOAD_DIRECTIVE       "upload_path"
# define REDIRECT_DIRECTIVE     "redirect"
# define INCLUDE_DIRECTIVE      "include"

# define PORT_DIRECTIVE_ARGS(num)       (num == 1)
# define HOST_DIRECTIVE_ARGS(num)       (num == 1)
# define NAME_DIRECTIVE_ARGS(num)       (num > 0)
# define ROOT_DIRECTIVE_ARGS(num)       (num == 1)
# define INDEX_DIRECTIVE_ARGS(num)      (num > 0)
# define ERROR_DIRECTIVE_ARGS(num)      (num > 1)
# define SIZE_DIRECTIVE_ARGS(num)       (num == 1)
# define METHOD_DIRECTIVE_ARGS(num)     (num > 0)
# define AUTOINDEX_DIRECTIVE_ARGS(num)  (num == 1)
# define CGI_DIRECTIVE_ARGS(num)        (num > 0)
# define UPLOAD_DIRECTIVE_ARGS(num)     (num == 1)
# define REDIRECT_DIRECTIVE_ARGS(num)   (num == 2)
# define INCLUDE_DIRECTIVE_ARGS(num)    (num == 1)

# define PORT_DIRECTIVE_POS(pos)        (pos == SERVER_CONTEXT)
# define HOST_DIRECTIVE_POS(pos)        (pos == SERVER_CONTEXT)
# define NAME_DIRECTIVE_POS(pos)        (pos == SERVER_CONTEXT)
# define ROOT_DIRECTIVE_POS(pos)        (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define INDEX_DIRECTIVE_POS(pos)       (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define ERROR_DIRECTIVE_POS(pos)       (pos == SERVER_CONTEXT)
# define SIZE_DIRECTIVE_POS(pos)        (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define METHOD_DIRECTIVE_POS(pos)      (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define AUTOINDEX_DIRECTIVE_POS(pos)   (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define CGI_DIRECTIVE_POS(pos)         (pos == LOCATION_CONTEXT)
# define UPLOAD_DIRECTIVE_POS(pos)      (pos != MAIN_CONTEXT && pos != TYPES_CONTEXT)
# define REDIRECT_DIRECTIVE_POS(pos)    (pos == LOCATION_CONTEXT)
# define INCLUDE_DIRECTIVE_POS(pos)     (true)

# define DEFAULT_PORT           "8080"
# define DEFAULT_HOST           "0.0.0.0"
# define DEFAULT_ROOT           "/var/www/html"
# define DEFAULT_INDEX          "index.html index.htm index.php"
# define DEFAULT_SIZE           "1M"
# define DEFAULT_METHOD         "GET POST DELETE"
# define DEFAULT_AUTOINDEX      "off"

# define WITE_SPACES    " \t\r\n\v\f"
# define NEW_LINE       '\n'

# define MAX_DEPTH  100

# define MAX_PORT   65535
# define MIN_PORT   0

# define MAX_STATUS_CODE  599
# define MIN_STATUS_CODE  100

// body size in megabytes
# define MAX_SIZE  100
# define MIN_SIZE  0

# define SUPPORTED_METHODS "GET POST DELETE"

# include <string>
# include <vector>
# include <poll.h>

# include "webserv/context.hpp"

namespace CORE {

// configuration file parser ***********************************************************************************
const Context* loadConfiguration(const std::string& configFilePath, Context* configuration, const size_t depth);

// server initialization *************************************************
size_t init(const Context* const configuration, std::vector<pollfd>& fds);

} // namespace CORE

# endif