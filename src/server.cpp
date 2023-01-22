# include <string>
# include <vector>

# include "server.hpp"
# include "definitions.hpp"

Server::Server(const std::vector<std::string>& contextArgs)
{
    if (contextArgs.empty() == false) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(SERVER_CONTEXT) + "' block");
    }

    this->host = DEFAULT_HOST;
    this->port = DEFAULT_PORT;
    this->name = DEFAULT_NAME;
}

// getters
const std::string& Server::getHost() const {
    return this->host;
}

const in_port_t& Server::getPort() const {
    return this->port;
}

const std::string& Server::getName() const {
    return this->name;
}

const std::vector<Location>& Server::getLocations() const {
    return this->locations;
}

std::vector<Location>& Server::getLocations() {
    return this->locations;
}

// setters
void Server::setHost(const std::vector<std::string>& directiveArgs) {
    if (directiveArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(HOST_DIRECTIVE) + "' directive.");
    }

    this->host = directiveArgs.front();
}

void Server::setPort(const std::vector<std::string>& directiveArgs) {
    if (directiveArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(PORT_DIRECTIVE) + "' directive.");
    }

    char *endptr = NULL;
    long port = strtol(directiveArgs.front().c_str(), &endptr, 10);
    if (*endptr != '\0' || port < 0 || port > MAX_PORT) {
        throw std::invalid_argument("Invalid port number '" + directiveArgs.front() + "' for '" + std::string(PORT_DIRECTIVE) + "' directive.");
    }

    this->port = static_cast<in_port_t>(port);
}

void Server::setName(const std::vector<std::string>& directiveArgs) {
    if (directiveArgs.size() != 1) {
        throw std::invalid_argument("Invalid number of arguments for '" + std::string(NAME_DIRECTIVE) + "' directive.");
    }

    this->name = directiveArgs.front();
}

void Server::addLocation(const std::vector<std::string>& directiveArgs) {
    try {
        this->locations.push_back(Location(directiveArgs));
    } catch (const std::invalid_argument& e) {
        throw e;
    }
}

Server::~Server() {
    this->host.clear();
    this->port = 0;
    this->name.clear();
    this->locations.clear();
}