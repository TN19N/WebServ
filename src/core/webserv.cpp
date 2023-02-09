# include "webserv/webserv.hpp"
# include "webserv/loadConfiguration.hpp"


# ifdef DEBUG
# include <iostream>
static void display(const Context* context, const size_t level) {
    const std::vector<std::string>& args = context->getArgs();
    std::cerr << std::string(level, '\t');
    for (size_t i = 0; i < args.size(); ++i) {
        std::cerr << "[" << args[i] << "] ";
    }
    std::cerr << std::endl;

    const std::map<const std::string, std::vector<std::string> >& directives = context->getDirectives();
    for (std::map<const std::string, std::vector<std::string> >::const_iterator it = directives.begin(); it != directives.end(); ++it) {
        std::cerr << std::string(level + 1, '\t') << "(" << it->first << ") : ";
        for (size_t i = 0; i < it->second.size(); ++i) {
            std::cerr << "[" << it->second[i] << "] ";
        }
        std::cerr << std::endl;
    }

    for (std::vector<Context*>::const_iterator it = context->getChildren().begin(); it != context->getChildren().end(); ++it) {
        display(*it, level + 1);
    }
}
# endif

// static member initialization
const WebServer* WebServer::instance = nullptr;

WebServer::WebServer(const std::string& configFilePath) 
    : configuration(loadConfiguration(configFilePath, nullptr, 1)) 
{
    # ifdef DEBUG
    std::cerr << " --------------------- configuration --------------------- " << std::endl;
    display(configuration, 0);
    # endif
}

const WebServer& WebServer::getInstance(const std::string& configFilePath) {
    if (WebServer::instance == nullptr) {
        try {
            WebServer::instance = new WebServer(configFilePath);
        } catch (const std::exception& e) {
            throw std::runtime_error("initialization failed: " + std::string(e.what()));
        }
    }
    return *(WebServer::instance);
}

void WebServer::run() const {
    // while (true);
}

WebServer::~WebServer() {
    delete this->configuration;
}