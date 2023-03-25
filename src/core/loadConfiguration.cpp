# include <string>
# include <fstream>
# include <algorithm>
# include <cstring>
# include <sstream>

# include "../../include/webserv/core.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/configuration.hpp"

# define WHITE_SPACE " \t\r\n\v\f"
# define NEW_LINE '\n'
# define ESCAPE '\\'

# define SUPPORTED_METHODS "GET POST DELETE"

# define MIN_PORT 0
# define MAX_PORT 65535

# define MIN_STATUS_CODE 100
# define MAX_STATUS_CODE 599

# define MAX_INCLUDE_DEPTH 100

static void checkRequirements(const bool argsNumber, const bool position, const std::string& name, const std::string& type) {
    if (argsNumber == false) {
        throw std::runtime_error("invalid number of arguments for '" + name + "' " + type);
    }

    if (position == false) {
        throw std::runtime_error("'" + name + "' " + type + " not allowed in this context");
    }
}

static bool isValidStatusCode(const std::string& str) {
    if (str.length() != 3) {
        return false;
    }

    char *end = NULL;
    long statusCode = strtol(str.c_str(), &end, 10);
    if (*end != '\0' || statusCode < MIN_STATUS_CODE || statusCode > MAX_STATUS_CODE) {
        return false;
    }
    return true;
}

static Context* addContext(const std::vector<std::string>& args, Context* currentContext) {
    if (args.empty()) {
        throw std::runtime_error("empty context name");
    }

    const std::string& contextName = args.at(0);
    const std::vector<std::string> contextArgs(args.begin() + 1, args.end());

    if (contextName == HTTP_CONTEXT) {
        checkRequirements(HTTP_CONTEXT_ARGS(contextArgs), HTTP_CONTEXT_POS(currentContext), contextName, "context");
        for (std::vector<Context*>::const_iterator it = currentContext->getChildren().begin(); it != currentContext->getChildren().end(); ++it) {
            if ((*it)->getName() == HTTP_CONTEXT) {
                throw std::runtime_error("only one '" + contextName + "' context allowed");
            }
        }
    } else if (contextName == SERVER_CONTEXT) {
        checkRequirements(SERVER_CONTEXT_ARGS(contextArgs), SERVER_CONTEXT_POS(currentContext), contextName, "context");
    } else if (contextName == LOCATION_CONTEXT) {
        checkRequirements(LOCATION_CONTEXT_ARGS(contextArgs), LOCATION_CONTEXT_POS(currentContext), contextName, "context");
        if (contextArgs.at(0).at(0) != '/') {
            throw std::runtime_error("invalid location '" + contextArgs.at(0) + "' in '" + contextName + "' context");
        }
    } else if (contextName == TYPES_CONTEXT) {
        checkRequirements(TYPES_CONTEXT_ARGS(contextArgs), TYPES_CONTEXT_POS(currentContext), contextName, "context");
    } else {
        throw std::runtime_error("unknown context '" + contextName + "'");
    }

    Context* newContext = new Context(contextName, contextArgs, currentContext);
    currentContext->addChild(newContext);
    return newContext;
}

static void addDirective(const std::vector<std::string>& args, Context* currentContext, const size_t depth) {
    if (args.empty()) {
        throw std::runtime_error("empty directive");
    }

    const std::string& directiveName = args.at(0);
    const std::vector<std::string> directiveArgs(args.begin() + 1, args.end());

    if (directiveName == PORT_DIRECTIVE) {
        checkRequirements(PORT_DIRECTIVE_ARGS(directiveArgs), PORT_DIRECTIVE_POS(currentContext), directiveName, "directive");

        char *end = NULL;
        const std::string portStr = directiveArgs.at(0);
        long port = strtol(portStr.c_str(), &end, 10);

        if (*end != '\0' || (portStr.at(0) == '0' && portStr.length() > 0) || port < MIN_PORT || port > MAX_PORT) {
            throw std::runtime_error("invalid port '" + portStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == HOST_DIRECTIVE) {
        checkRequirements(HOST_DIRECTIVE_ARGS(directiveArgs), HOST_DIRECTIVE_POS(currentContext), directiveName, "directive");
    } else if (directiveName == NAME_DIRECTIVE) {
        checkRequirements(NAME_DIRECTIVE_ARGS(directiveArgs), NAME_DIRECTIVE_POS(currentContext), directiveName, "directive");
    } else if (directiveName == ROOT_DIRECTIVE) {
        checkRequirements(ROOT_DIRECTIVE_ARGS(directiveArgs), ROOT_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::string& rootStr = directiveArgs.at(0);

        if (rootStr.at(0) != '/') {
            throw std::runtime_error("invalid root '" + rootStr + "' in '" + directiveName + "' directive");
        }
        if (rootStr.at(rootStr.size() - 1) == '/' && rootStr.length() > 1) {
            std::vector<std::string> newDirectiveArgs;
            newDirectiveArgs.push_back(rootStr.substr(0, rootStr.length() - 1));
            currentContext->addDirective(directiveName, newDirectiveArgs);
            return ;
        }
    } else if (directiveName == INDEX_DIRECTIVE) {
        checkRequirements(INDEX_DIRECTIVE_ARGS(directiveArgs), INDEX_DIRECTIVE_POS(currentContext), directiveName, "directive");
    } else if (directiveName == ERROR_DIRECTIVE) {
        checkRequirements(ERROR_DIRECTIVE_ARGS(directiveArgs), ERROR_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::vector<std::string> errorFilePath(directiveArgs.end() - 1, directiveArgs.end());

        if (errorFilePath.at(0).at(0) != '/') {
            throw std::runtime_error("invalid error page '" + errorFilePath.at(0) + "' in '" + directiveName + "' directive");
        }

        for (std::vector<std::string>::const_iterator it = directiveArgs.begin(); it != directiveArgs.end() - 1; ++it) {
            if (isValidStatusCode(*it) == false) {
                throw std::runtime_error("invalid status code '" + (*it) + "' in '" + directiveName + "' directive");
            }
            currentContext->addDirective((*it), errorFilePath);
        }
        return ;
    } else if (directiveName == SIZE_DIRECTIVE) {
        checkRequirements(SIZE_DIRECTIVE_ARGS(directiveArgs), SIZE_DIRECTIVE_POS(currentContext), directiveName, "directive");

        char *end = NULL;
        const std::string sizeStr = directiveArgs.at(0);
        long size = strtol(sizeStr.c_str(), &end, 10);
        if (*end != 'M' || *(end + 1) != '\0' || (sizeStr.length() > 2 && sizeStr.at(0) == '0') || size < 0) {
            throw std::runtime_error("invalid size '" + sizeStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == METHOD_DIRECTIVE) {
        checkRequirements(METHOD_DIRECTIVE_ARGS(directiveArgs), METHOD_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::vector<std::string> supportedMethods = CORE::split(SUPPORTED_METHODS, WHITE_SPACE);
        for (std::vector<std::string>::const_iterator it = directiveArgs.begin(); it != directiveArgs.end(); ++it) {
            if (std::find(supportedMethods.begin(), supportedMethods.end(), (*it)) == supportedMethods.end()) {
                throw std::runtime_error("invalid method '" + (*it) + "' in '" + directiveName + "' directive");
            }
            if (std::count(directiveArgs.begin(), directiveArgs.end(), (*it)) > 1) {
                throw std::runtime_error("duplicate method '" + (*it) + "' in '" + directiveName + "' directive");
            }
        }
    } else if (directiveName == AUTOINDEX_DIRECTIVE) {
        checkRequirements(AUTOINDEX_DIRECTIVE_ARGS(directiveArgs), AUTOINDEX_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::string& autoindexStr = directiveArgs.at(0);
        if (autoindexStr != "on" && autoindexStr != "off") {
            throw std::runtime_error("invalid autoindex '" + autoindexStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == CGI_DIRECTIVE) {
        checkRequirements(CGI_DIRECTIVE_ARGS(directiveArgs), CGI_DIRECTIVE_POS(currentContext), directiveName, "directive");

        for (size_t i = 1; i < directiveArgs.size(); i += 2) {
            if (directiveArgs.at(i).at(0) != '.' || directiveArgs.at(i).length() < 2 || std::count(directiveArgs.at(i).begin(), directiveArgs.at(i).end(), '.') > 1) {
                throw std::runtime_error("invalid extension '" + directiveArgs.at(i) + "' in '" + directiveName + "' directive");
            }
        }
    } else if (directiveName == UPLOAD_DIRECTIVE) {
        checkRequirements(UPLOAD_DIRECTIVE_ARGS(directiveArgs), UPLOAD_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::string& uploadPathStr = directiveArgs.at(0);
        if (uploadPathStr.at(0) != '/') {
            throw std::runtime_error("invalid upload path '" + uploadPathStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == REDIRECT_DIRECTIVE) {
        checkRequirements(REDIRECT_DIRECTIVE_ARGS(directiveArgs), REDIRECT_DIRECTIVE_POS(currentContext), directiveName, "directive");

        const std::string& redirectStatusCode = directiveArgs.at(0);
        if (isValidStatusCode(redirectStatusCode) == false) {
            throw std::runtime_error("invalid status code '" + redirectStatusCode + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == INCLUDE_DIRECTIVE) {
        checkRequirements(INCLUDE_DIRECTIVE_ARGS(directiveArgs), INCLUDE_DIRECTIVE_POS(currentContext), directiveName, "directive");
        CORE::loadConfiguration(directiveArgs.at(0), currentContext, depth + 1);
    } else if (currentContext->getName() == TYPES_CONTEXT) {
        std::vector<std::string> mimeType = CORE::split(directiveName, WHITE_SPACE);
        for (std::vector<std::string>::const_iterator it = directiveArgs.begin(); it != directiveArgs.end(); ++it) {
            if (std::find((*it).begin(), (*it).end(), '.') != (*it).end()) {
                throw std::runtime_error("invalid extension '" + (*it) + "' in '" + directiveName + "' directive");
            }
            currentContext->getParent()->addDirective("." + (*it), mimeType);
        }
        return ;
    } else {
        throw std::runtime_error("unknown directive '" + directiveName + "'");
    }
    currentContext->addDirective(directiveName, directiveArgs);
}

const Context* CORE::loadConfiguration(const std::string& configFilePath, Context* configuration, const size_t depth) {
    if (depth > MAX_INCLUDE_DEPTH) {
        throw std::runtime_error("too many nested includes");
    }
    
    std::ifstream configFile(configFilePath.c_str());
    if (configFile.is_open() == false) {
        throw std::runtime_error("open('" + configFilePath + "'): " + strerror(errno));
    }

    Context* currentContext = configuration;
    std::string token;
    size_t line = 1;
    char c = configFile.get();

    try {
        while (configFile.good()) {
            switch (c) {
                case '{':
                    currentContext = addContext(CORE::split(token, WHITE_SPACE), currentContext);
                    token.clear();
                    break;
                case '}':
                    if (currentContext->getName() == MAIN_CONTEXT) {
                        throw std::runtime_error("unexpected '}'");
                    }
                    currentContext = currentContext->getParent();
                    break;
                case ';':
                    addDirective(CORE::split(token, WHITE_SPACE), currentContext, depth);
                    token.clear();
                    break;
                case '#':
                    while (configFile.good() && configFile.get() != NEW_LINE);
                    ++line;
                    break;
                case NEW_LINE:
                    ++line;
                    break;
                case ESCAPE:
                    token += c;
                    c = configFile.get();
                    token += c;
                    break;
                default:
                    token += c;
            }
            c = configFile.get();
        }
    } catch (const std::exception& e) {
        std::stringstream lineStr;
        lineStr << line;
        throw std::runtime_error("('" + configFilePath + "' line: " + lineStr.str() + "): " + e.what());
    }

    if (token.find_first_not_of(WHITE_SPACE) != std::string::npos) {
        std::stringstream lineStr;
        lineStr << line;
        throw std::runtime_error("('" + configFilePath + "' line: " + lineStr.str() + "): unexpected end of file");
    }

    return configuration;
}