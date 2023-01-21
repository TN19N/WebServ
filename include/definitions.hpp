# ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

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

// directive names
# define HOST_DIRECTIVE  "host"
# define PORT_DIRECTIVE  "port"
# define NAME_DIRECTIVE  "name"
# define ROOT_DIRECTIVE  "root"
# define INDEX_DIRECTIVE "index"

// values
# define MAX_PORT 65535

# endif