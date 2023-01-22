# ifndef PARSING_HPP
# define PARSING_HPP

// utils
const std::vector<std::string> split(const std::string& str, const std::string& delimiters);

// config file
void parseConfigFile(const std::string& configFilePath);

# endif