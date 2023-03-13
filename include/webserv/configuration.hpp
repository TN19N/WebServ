# ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# define MAIN_CONTEXT       "main"
# define HTTP_CONTEXT       "http"
# define SERVER_CONTEXT     "server"
# define LOCATION_CONTEXT   "location"
# define TYPES_CONTEXT      "types"

# define HTTP_CONTEXT_ARGS(args)     (args.size() == 0)
# define SERVER_CONTEXT_ARGS(args)   (args.size() == 0)
# define LOCATION_CONTEXT_ARGS(args) (args.size() == 1)
# define TYPES_CONTEXT_ARGS(args)    (args.size() == 0)

# define HTTP_CONTEXT_POS(context)     (context->getName() == MAIN_CONTEXT)
# define SERVER_CONTEXT_POS(context)   (context->getName() == HTTP_CONTEXT)
# define LOCATION_CONTEXT_POS(context) (context->getName() == SERVER_CONTEXT)
# define TYPES_CONTEXT_POS(context)    (context->getName() == HTTP_CONTEXT)

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

# define PORT_DIRECTIVE_ARGS(args)       (args.size() == 1)
# define HOST_DIRECTIVE_ARGS(args)       (args.size() == 1)
# define NAME_DIRECTIVE_ARGS(args)       (args.size() > 0)
# define ROOT_DIRECTIVE_ARGS(args)       (args.size() == 1)
# define INDEX_DIRECTIVE_ARGS(args)      (args.size() > 0)
# define ERROR_DIRECTIVE_ARGS(args)      (args.size() > 1)
# define SIZE_DIRECTIVE_ARGS(args)       (args.size() == 1)
# define METHOD_DIRECTIVE_ARGS(args)     (args.size() > 0)
# define AUTOINDEX_DIRECTIVE_ARGS(args)  (args.size() == 1)
# define CGI_DIRECTIVE_ARGS(args)        (args.size() % 2 == 0 && args.size() > 0)
# define UPLOAD_DIRECTIVE_ARGS(args)     (args.size() == 1)
# define REDIRECT_DIRECTIVE_ARGS(args)   (args.size() == 2)
# define INCLUDE_DIRECTIVE_ARGS(args)    (args.size() == 1)

# define PORT_DIRECTIVE_POS(context)        (context->getName() == SERVER_CONTEXT)
# define HOST_DIRECTIVE_POS(context)        (context->getName() == SERVER_CONTEXT)
# define NAME_DIRECTIVE_POS(context)        (context->getName() == SERVER_CONTEXT)
# define ROOT_DIRECTIVE_POS(context)        (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define INDEX_DIRECTIVE_POS(context)       (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define ERROR_DIRECTIVE_POS(context)       (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define SIZE_DIRECTIVE_POS(context)        (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define METHOD_DIRECTIVE_POS(context)      (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define AUTOINDEX_DIRECTIVE_POS(context)   (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define CGI_DIRECTIVE_POS(context)         (context->getName() == LOCATION_CONTEXT)
# define UPLOAD_DIRECTIVE_POS(context)      (context->getName() != MAIN_CONTEXT && context->getName() != TYPES_CONTEXT)
# define REDIRECT_DIRECTIVE_POS(context)    (context->getName() == LOCATION_CONTEXT)
# define INCLUDE_DIRECTIVE_POS(context)     (true)

# define DEFAULT_PORT           "8080"
# define DEFAULT_HOST           "0.0.0.0"
# define DEFAULT_ROOT           "/var/www/html"
# define DEFAULT_INDEX          "index.html index.htm index.php"
# define DEFAULT_SIZE           "1M"
# define DEFAULT_METHOD         "GET POST DELETE"
# define DEFAULT_AUTOINDEX      "off"
# define DEFAULT_MIME_TYPE		"application/octet-stream"


# endif