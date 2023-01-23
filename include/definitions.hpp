# ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

// DEBUG
// # define DEBUG_CONFIG_FILE_PARSER
// # define DEBUG_SERVER_INIT

// general
# define WHITE_SPACES "\t\n\v\f\r "
# define NEW_LINE '\n'

// default values for location block
# define DEFAULT_INDEX "index.html"
# define DEFAULT_ROOT  "/var/www/html"

// default values for server block
# define DEFAULT_HOST "0.0.0.0"
# define DEFAULT_PORT 8080
# define DEFAULT_NAME ""

// default config file
# define DEFAULT_CONFIG_FILE "./default.cfg"

// context names
# define SERVER_CONTEXT   "server"
# define LOCATION_CONTEXT "location"

// context levels (where each context can be used)
# define SERVER_CONTEXT_LEVEL   0
# define LOCATION_CONTEXT_LEVEL 1

// directive names
# define HOST_DIRECTIVE  "host"
# define PORT_DIRECTIVE  "port"
# define NAME_DIRECTIVE  "name"
# define ROOT_DIRECTIVE  "root"
# define INDEX_DIRECTIVE "index"

// directive levels (where each directive can be used)
# define HOST_DIRECTIVE_LEVEL  1
# define PORT_DIRECTIVE_LEVEL  1
# define NAME_DIRECTIVE_LEVEL  1
# define ROOT_DIRECTIVE_LEVEL  2
# define INDEX_DIRECTIVE_LEVEL 2

// values
# define MAX_PORT 65535
# define BACKLOG  10

# endif