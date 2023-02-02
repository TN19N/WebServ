# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <iostream>

# include "definitions.hpp"

const std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> tokens;
    std::string::size_type start = str.find_first_not_of(delimiters);
    std::string::size_type end = str.find_first_of(delimiters, start);
    while (start != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delimiters, end);
        end = str.find_first_of(delimiters, start);
    }
    return tokens;
}

void loadMimeTypes(const std::string& mimeTypesFilePath, std::map<std::string, std::string>& mimeType) {
    std::ifstream mimeTypesFile(mimeTypesFilePath);
    if (mimeTypesFile.is_open() == false) {
        std::cerr << "webserv: failed to load mime types '" << mimeTypesFilePath << "' : " << strerror(errno) << "." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (mimeTypesFile.eof() == false) {
        std::string line;
        std::getline(mimeTypesFile, line);
        std::vector<std::string> tokens = split(line, WHITE_SPACE);
        for (size_t i = 1; i < tokens.size(); ++i) {
            mimeType[tokens[i]] = tokens[0];
        }
    }
    mimeTypesFile.close();

    # ifdef DEBUG_MIME_TYPES_PARSER
    std::cerr << " DEBUG_MIME_TYPES_PARSER ----------------------------------------------------- " << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = mimeType.begin(); it != mimeType.end(); ++it) {
        std::cerr << "[" << it->first << "] : " << it->second << std::endl;
    }
    std::cerr << " ----------------------------------------------------------------------------- " << std::endl;
    # endif
}