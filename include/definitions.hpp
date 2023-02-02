# ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

# define DEBUG_CONFIG_FILE_PARSER

# define DEFAULT_CONFIG_FILE "default.cfg"

# define WHITE_SPACE " \t\n\r\f\v"
# define NEW_LINE    '\n'

# define MAIN_CONTEXT       "main"
# define HTTP_CONTEXT       "http"
# define SERVER_CONTEXT     "server"
# define LOCATION_CONTEXT   "location"

# define HTTP_CONTEXT_POS(pos)      (pos != MAIN_CONTEXT)
# define SERVER_CONTEXT_POS(pos)    (pos != HTTP_CONTEXT)
# define LOCATION_CONTEXT_POS(pos)  (pos != SERVER_CONTEXT)

# define HTTP_CONTEXT_ARGS(args)        (args != 0)
# define SERVER_CONTEXT_ARGS(args)      (args != 0)
# define LOCATION_CONTEXT_ARGS(args)    (args != 1)

# define PORT_DIRECTIVE             "port"
# define HOST_DIRECTIVE             "host"
# define NAME_DIRECTIVE             "name"
# define INDEX_DIRECTIVE            "index"
# define ROOT_DIRECTIVE             "root"
# define AUTOINDEX_DIRECTIVE        "autoindex"
# define ALLOWED_METHODS_DIRECTIVE  "allowed_methods"
# define UPLOAD_PATH_DIRECTIVE      "upload_path"
# define CGI_DIRECTIVE              "cgi"
# define REDIRECT_DIRECTIVE         "redirect"
# define BODY_SIZE_DIRECTIVE        "client_max_body_size"
# define ERROR_PAGE_DIRECTIVE       "error_page"

# define PORT_DIRECTIVE_POS(pos)            (pos != SERVER_CONTEXT)
# define HOST_DIRECTIVE_POS(pos)            (pos != SERVER_CONTEXT)
# define NAME_DIRECTIVE_POS(pos)            (pos != SERVER_CONTEXT)
# define INDEX_DIRECTIVE_POS(pos)           (pos == MAIN_CONTEXT)
# define ROOT_DIRECTIVE_POS(pos)            (pos == MAIN_CONTEXT)
# define AUTOINDEX_DIRECTIVE_POS(pos)       (pos == MAIN_CONTEXT)
# define ALLOWED_METHODS_DIRECTIVE_POS(pos) (pos == MAIN_CONTEXT)
# define UPLOAD_PATH_DIRECTIVE_POS(pos)     (pos != LOCATION_CONTEXT)
# define CGI_DIRECTIVE_POS(pos)             (pos != LOCATION_CONTEXT)
# define REDIRECT_DIRECTIVE_POS(pos)        (pos != LOCATION_CONTEXT)
# define BODY_SIZE_DIRECTIVE_POS(pos)       (pos == MAIN_CONTEXT)
# define ERROR_PAGE_DIRECTIVE_POS(pos)      (pos == MAIN_CONTEXT)

# define PORT_DIRECTIVE_ARGS(args)              (args != 1)
# define HOST_DIRECTIVE_ARGS(args)              (args != 1)
# define NAME_DIRECTIVE_ARGS(args)              (args < 1)
# define INDEX_DIRECTIVE_ARGS(args)             (args < 1)
# define ROOT_DIRECTIVE_ARGS(args)              (args != 1)
# define AUTOINDEX_DIRECTIVE_ARGS(args)         (args != 1)
# define ALLOWED_METHODS_DIRECTIVE_ARGS(args)   (args < 1)
# define UPLOAD_PATH_DIRECTIVE_ARGS(args)       (args != 1)
# define CGI_DIRECTIVE_ARGS(args)               (args < 1)
# define REDIRECT_DIRECTIVE_ARGS(args)          (args != 2)
# define BODY_SIZE_DIRECTIVE_ARGS(args)         (args != 1)
# define ERROR_PAGE_DIRECTIVE_ARGS(args)        (args < 2 || args % 2 == 1)

# define DEFAULT_PORT                "8080"
# define DEFAULT_HOST                "0.0.0.0"
# define DEFAULT_NAME                ""
# define DEFAULT_INDEX               "index.html index.htm index.php"
# define DEFAULT_ROOT                "/var/www/html"
# define DEFAULT_AUTOINDEX           "on"
# define DEFAULT_ALLOWED_METHODS     "GET POST DELETE"
# define DEFAULT_BODY_SIZE           "1M"

# define MAX_PORT                    65535

# endif