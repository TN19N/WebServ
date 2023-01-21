# include <fstream>
# include <iostream>
# include <string>
# include <vector>

# include "definitions.hpp"
# include "parser.hpp"
# include "webServ.hpp"

static void panic(const std::string& msg) {
    std::cerr << "webserv: invalid config file : " << msg << std::endl;
    exit(EXIT_FAILURE);
}

static void addNewContext(const std::vector<std::string>& context) {
    if (context.empty() == true) {
        panic("empty context.");
    }

    std::cerr << "context: ";
    for (std::vector<std::string>::const_iterator it = context.begin(); it != context.end(); ++it) {
        std::cerr << "(" << *it << ") ";
    }
    std::cerr << std::endl;

    std::string              contextName = context.front();
    std::vector<std::string> contextArgs(context.begin() + 1, context.end());

    if (contextName == SERVER_CONTEXT) {
        try { WebServ::servers.push_back(Server(contextArgs)); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (contextName == LOCATION_CONTEXT) {
        try { WebServ::servers.back().addLocation(contextArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else {
        panic("unknown context '" + contextName + "'.");
    }
}

static void addNewDirective(const std::vector<std::string>& directive) {
    if (directive.empty() == true) {
        panic("empty directive.");
    }

    std::cerr << "directive: ";
    for (std::vector<std::string>::const_iterator it = directive.begin(); it != directive.end(); ++it) {
        std::cerr << "(" << *it << ") ";
    }
    std::cerr << std::endl;

    std::string              directiveName = directive.front();
    std::vector<std::string> directiveArgs(directive.begin() + 1, directive.end());
    Server&                  currentServer = WebServ::servers.back();

    if (directiveName == HOST_DIRECTIVE) {
        try { WebServ::servers.back().setHost(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == PORT_DIRECTIVE) {
        try { WebServ::servers.back().setPort(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == NAME_DIRECTIVE) {
        try { WebServ::servers.back().setName(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == INDEX_DIRECTIVE) {
        try { WebServ::servers.back().getLocations().back().setIndex(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == ROOT_DIRECTIVE) {
        try { WebServ::servers.back().getLocations().back().setRoot(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else {
        panic("unknown directive '" + directiveName + "'.");
    }
}

void parseConfigFile(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (configFile.is_open() == false) {
        panic(configFilePath + " : " + strerror(errno));
    }

    std::string token;
    while (configFile.eof() == false) {
        const char c = configFile.get();
        switch (c) {
            case '{' :
                addNewContext(split(token, WHITE_SPACES));
                token.clear();
                break;
            case ';' :
                addNewDirective(split(token, WHITE_SPACES));
                token.clear();
                break;
            case '}' :
                if (token.find_first_not_of(WHITE_SPACES) != std::string::npos) {
                    panic("unexpected '}'.");
                }
                break;
            default :
                token += c;
        }
    }
}