# include <fstream>
# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

# include "definitions.hpp"
# include "parser.hpp"
# include "webServ.hpp"
# include "server.hpp"
# include "location.hpp"

static void panic(const std::string& msg) {
    std::cerr << "webserv: invalid config file : " << msg << std::endl;
    exit(EXIT_FAILURE);
}

static void addNewContext(const std::vector<std::string>& context, const size_t& level) {
    if (context.empty() == true) {
        panic("empty context.");
    }

    const std::string&              contextName = context.front();
    const std::vector<std::string>  contextArgs(context.begin() + 1, context.end());

    if (contextName == SERVER_CONTEXT) {
        if (level != SERVER_CONTEXT_LEVEL) {
            panic("'" + std::string(SERVER_CONTEXT) + "' context should be at the root level.");
        }
        try { WebServ::servers.push_back(Server(contextArgs)); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (contextName == LOCATION_CONTEXT) {
        if (level != LOCATION_CONTEXT_LEVEL) {
            panic("'" + std::string(LOCATION_CONTEXT) + "' context should in a '" + SERVER_CONTEXT + "' context.");
        }
        try { WebServ::servers.back().addLocation(contextArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else {
        panic("unknown context '" + contextName + "'.");
    }
}

static void addNewDirective(const std::vector<std::string>& directive, const size_t& level, std::vector<std::string>& passDirectives) {
    if (directive.empty() == true) {
        panic("empty directive.");
    }

    if (std::find(passDirectives.begin(), passDirectives.end(), directive.front()) != passDirectives.end()) {
        panic("directive '" + directive.front() + "' already defined.");
    } else {
        passDirectives.push_back(directive.front());
    }

    const std::string&             directiveName = directive.front();
    const std::vector<std::string> directiveArgs(directive.begin() + 1, directive.end());
    Server&                        currentServer = WebServ::servers.back();

    if (directiveName == HOST_DIRECTIVE) {
        if (level != HOST_DIRECTIVE_LEVEL) {
            panic("'" + std::string(HOST_DIRECTIVE) + "' directive should in a '" + SERVER_CONTEXT + "' context.");
        }
        try { WebServ::servers.back().setHost(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == PORT_DIRECTIVE) {
        if (level != PORT_DIRECTIVE_LEVEL) {
            panic("'" + std::string(PORT_DIRECTIVE) + "' directive should in a '" + SERVER_CONTEXT + "' context.");
        }
        try { WebServ::servers.back().setPort(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == NAME_DIRECTIVE) {
        if (level != NAME_DIRECTIVE_LEVEL) {
            panic("'" + std::string(NAME_DIRECTIVE) + "' directive should in a '" + SERVER_CONTEXT + "' context.");
        }
        try { WebServ::servers.back().setName(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == INDEX_DIRECTIVE) {
        if (level != INDEX_DIRECTIVE_LEVEL) {
            panic("'" + std::string(INDEX_DIRECTIVE) + "' directive should in a '" + LOCATION_CONTEXT + "' context.");
        }
        try { WebServ::servers.back().getLocations().back().setIndex(directiveArgs); } catch (const std::invalid_argument& e) {
            panic(e.what());
        }
    } else if (directiveName == ROOT_DIRECTIVE) {
        if (level != ROOT_DIRECTIVE_LEVEL) {
            panic("'" + std::string(ROOT_DIRECTIVE) + "' directive should in a '" + LOCATION_CONTEXT + "' context.");
        }
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
        panic(configFilePath + " (" + strerror(errno) + ").");
    }

    std::string token;
    size_t level = 0;
    std::vector<std::string> passDirectives;
    while (configFile.eof() == false) {
        const char c = configFile.get();
        switch (c) {
            case '{' :
                addNewContext(split(token, WHITE_SPACES), level);
                token.clear();
                ++level;
                break;
            case ';' :
                addNewDirective(split(token, WHITE_SPACES), level, passDirectives);
                token.clear();
                break;
            case '}' :
                if (token.find_first_not_of(WHITE_SPACES) != std::string::npos || level == 0) {
                    panic("unexpected '}'.");
                }
                passDirectives.clear();
                --level;
                break;
            case '#' :
                while (configFile.get() != NEW_LINE && configFile.eof() == false);
                break;
            case EOF :
                break;
            default :
                token += c;
        }
    }

    if (token.find_first_not_of(WHITE_SPACES) != std::string::npos) {
        panic("unexpected end of file.");
    }

    configFile.close();

    # ifdef DEBUG_CONFIG_FILE_PARSER // DEBUG ----------------------------------------
        for (int i = 0; i < WebServ::servers.size(); ++i) {
            std::cerr << " ---------------------------------------- " << std::endl;
            Server& server = WebServ::servers[i];
            std::cerr << "Server :: " << std::endl;
            std::cerr << "  host: " << server.getHost() << std::endl;
            std::cerr << "  port: " << server.getPort() << std::endl;
            std::cerr << "  name: " << server.getName() << std::endl;
            std::vector<Location>& locations = server.getLocations();
            for (int j = 0; j < locations.size(); ++j) {
                Location& location = locations[j];
                std::cerr << "  Location :: " << std::endl;
                std::cerr << "    match: " << location.getMatch() << std::endl;
                std::cerr << "    root: " << location.getRoot() << std::endl;
                std::cerr << "    index: " << location.getIndex() << std::endl;
            }
        }
    # endif // -----------------------------------------------------------------------
}