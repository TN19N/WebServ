# include <string>
# include <vector>

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