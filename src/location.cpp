# include "location.hpp"

# include "definitions.hpp"

Location::Location(const std::vector<std::string>& contextArgs) {
    if (contextArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(LOCATION_CONTEXT) + "' context.");
    }

    this->match = contextArgs.front();
    this->index = DEFAULT_INDEX;
    this->root = DEFAULT_ROOT;
}

// getters
const std::string& Location::getMatch() const {
    return this->match;
}

const std::string& Location::getIndex() const {
    return this->index;
}

const std::string& Location::getRoot() const {
    return this->root;
}

// setters
void Location::setIndex(const std::vector<std::string>& directiveArgs) {
    if (directiveArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(INDEX_DIRECTIVE) + "' directive.");
    }

    this->index = directiveArgs.front();
}

void Location::setRoot(const std::vector<std::string>& directiveArgs) {
    if (directiveArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(ROOT_DIRECTIVE) + "' directive.");
    }

    this->root = directiveArgs.front();
}

Location::~Location() {
    this->match.clear();
    this->index.clear();
    this->root.clear();
}