# ifndef PARSER_HPP
# define PARSER_HPP

# include "context.hpp"

// utils
const std::vector<std::string> split(const std::string& str, const std::string& delimiters);

// config file
void parseConfigFile(const std::string& configFilePath, Context* mainContext);

# endif