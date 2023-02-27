# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <algorithm>

# include "webserv/context.hpp"
# include "webserv/core.hpp"

static std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
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

static Context* loadContext(const std::vector<std::string>& args, Context* parent) {
    Context* context = new Context(args, parent);
    
    const std::string& contextName = args.front();

    if (contextName == MAIN_CONTEXT) {
        context->addDirective(INDEX_DIRECTIVE, split(DEFAULT_INDEX, WITE_SPACES));
        context->addDirective(ROOT_DIRECTIVE, split(DEFAULT_ROOT, WITE_SPACES));
        context->addDirective(SIZE_DIRECTIVE, split(DEFAULT_SIZE, WITE_SPACES));
        context->addDirective(METHOD_DIRECTIVE, split(DEFAULT_METHOD, WITE_SPACES));
        context->addDirective(AUTOINDEX_DIRECTIVE, split(DEFAULT_AUTOINDEX, WITE_SPACES));
    } else if (contextName == SERVER_CONTEXT) {
        context->addDirective(PORT_DIRECTIVE, split(DEFAULT_PORT, WITE_SPACES));
        context->addDirective(HOST_DIRECTIVE, split(DEFAULT_HOST, WITE_SPACES));
    }

    return context;
}

static void checkRequirements(const bool argsNumber, const bool position, const std::string& name, const std::string& type) {
    if (argsNumber == false) {
        throw std::runtime_error("invalid number of arguments for '" + name + "' " + type);
    }
    if (position == false) {
        throw std::runtime_error("'" + name + "' " + type + " not allowed in this context");
    }
}

static Context* addNewContext(const std::vector<std::string>& args, Context* currentContext) {
    if (args.empty() == true) {
        throw std::runtime_error("empty context");
    }

    const std::string&              contextName = args.front();
    const std::vector<std::string>  contextArgs(args.begin() + 1, args.end());

    if (contextName == HTTP_CONTEXT) {
        checkRequirements(HTTP_CONTEXT_ARGS(contextArgs.size()), HTTP_CONTEXT_PARENT(currentContext->getName()), contextName, "context");
        const std::vector<Context*> children = currentContext->getChildren();
        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]->getName() == contextName) {
                throw std::runtime_error("only one '" + contextName + "' context allowed");
            }
        }
    } else if (contextName == SERVER_CONTEXT) {
        checkRequirements(SERVER_CONTEXT_ARGS(contextArgs.size()), SERVER_CONTEXT_PARENT(currentContext->getName()), contextName, "context");
    } else if (contextName == LOCATION_CONTEXT) {
        checkRequirements(LOCATION_CONTEXT_ARGS(contextArgs.size()), LOCATION_CONTEXT_PARENT(currentContext->getName()), contextName, "context");
        if (contextArgs[0].front() != '/') {
            throw std::runtime_error("invalid location '" + contextArgs[0] + "' in '" + contextName + "' context");
        }
    } else if (contextName == TYPES_CONTEXT) {
        checkRequirements(TYPES_CONTEXT_ARGS(contextArgs.size()), TYPES_CONTEXT_PARENT(currentContext->getName()), contextName, "context");
    } else {
        throw std::runtime_error("unknown context '" + contextName + "'");
    }

    Context* newContext = loadContext(args, currentContext);
    currentContext->addChild(newContext);
    return newContext;
}

static bool isValidStatusCode(const std::string& statusCode) {
    if (statusCode.length() != 3) {
        return false;
    }
    char *end = nullptr;
    long code = std::strtol(statusCode.c_str(), &end, 10);
    if (*end != '\0' || code < MIN_STATUS_CODE || code > MAX_STATUS_CODE) {
        return false;
    }
    return true;
}

static void addNewDirective(const std::vector<std::string>& args, Context* currentContext, const size_t depth) {
    if (args.empty() == true) {
        throw std::runtime_error("empty directive");
    }

    const std::string&             directiveName = args.front();
    const std::vector<std::string> directiveArgs(args.begin() + 1, args.end());

    if (directiveName == PORT_DIRECTIVE) {
        checkRequirements(PORT_DIRECTIVE_ARGS(directiveArgs.size()), PORT_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        char* end = nullptr;
        const std::string& portStr = directiveArgs.front();
        long port = std::strtol(portStr.c_str(), &end, 10);
        if (*end != '\0' || (portStr.length() > 1 && portStr.front() == '0') || port < MIN_PORT || port > MAX_PORT) {
            throw std::runtime_error("invalid port '" + portStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == HOST_DIRECTIVE) {
        checkRequirements(HOST_DIRECTIVE_ARGS(directiveArgs.size()), HOST_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        struct sockaddr_storage addr;
        const std::string& hostStr = directiveArgs.front();
        if (inet_pton(AF_INET, hostStr.c_str(), &addr) != 1 && inet_pton(AF_INET6, hostStr.c_str(), &addr) != 1) {
            throw std::runtime_error("invalid host '" + hostStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == NAME_DIRECTIVE) {
        checkRequirements(NAME_DIRECTIVE_ARGS(directiveArgs.size()), NAME_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
    } else if (directiveName == ROOT_DIRECTIVE) {
        checkRequirements(ROOT_DIRECTIVE_ARGS(directiveArgs.size()), ROOT_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::string& rootStr = directiveArgs.front();
        if (rootStr.front() != '/') {
            throw std::runtime_error("invalid root '" + rootStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == INDEX_DIRECTIVE) {
        checkRequirements(INDEX_DIRECTIVE_ARGS(directiveArgs.size()), INDEX_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
    } else if (directiveName == ERROR_DIRECTIVE) {
        checkRequirements(ERROR_DIRECTIVE_ARGS(directiveArgs.size()), ERROR_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::string& errorPageStr = directiveArgs.back();
        if (errorPageStr.front() != '/') {
            throw std::runtime_error("invalid error page '" + errorPageStr + "' in '" + directiveName + "' directive");
        }
        const std::vector<std::string> errorFilePath(directiveArgs.end() - 1, directiveArgs.end());
        for (size_t i = 0; i < directiveArgs.size() - 1; ++i) {
            if (isValidStatusCode(directiveArgs[i]) == false) {
                throw std::runtime_error("invalid status code '" + directiveArgs[i] + "' in '" + directiveName + "' directive");
            }
            currentContext->addDirective(directiveArgs[i], errorFilePath);
        }
        return ;
    } else if (directiveName == SIZE_DIRECTIVE) {
        checkRequirements(SIZE_DIRECTIVE_ARGS(directiveArgs.size()), SIZE_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        char* end = nullptr;
        const std::string& sizeStr = directiveArgs.front();
        long size = std::strtol(sizeStr.c_str(), &end, 10);
        if (*end != 'M' || *(end + 1) != '\0' || (sizeStr.length() > 1 && sizeStr.front() == '0') || size < MIN_SIZE || size > MAX_SIZE) {
            throw std::runtime_error("invalid size '" + sizeStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == METHOD_DIRECTIVE) {
        checkRequirements(METHOD_DIRECTIVE_ARGS(directiveArgs.size()), METHOD_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::vector<std::string>& supported_methods = split(SUPPORTED_METHODS, WITE_SPACES);
        for (size_t i = 0; i < directiveArgs.size(); ++i) {
            if (std::find(supported_methods.begin(), supported_methods.end(), directiveArgs[i]) == supported_methods.end()) {
                throw std::runtime_error("invalid method '" + directiveArgs[i] + "' in '" + directiveName + "' directive");
            }
        }
    } else if (directiveName == AUTOINDEX_DIRECTIVE) {
        checkRequirements(AUTOINDEX_DIRECTIVE_ARGS(directiveArgs.size()), AUTOINDEX_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::string& autoindexStr = directiveArgs.front();
        if (autoindexStr != "on" && autoindexStr != "off") {
            throw std::runtime_error("invalid autoindex '" + autoindexStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == CGI_DIRECTIVE) {
        checkRequirements(CGI_DIRECTIVE_ARGS(directiveArgs.size()), CGI_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        for (size_t i = 0; i < directiveArgs.size(); ++i) {
            if (directiveArgs[i].front() != '.' || std::count(directiveArgs[i].begin(), directiveArgs[i].end(), '.') != 1) {
                throw std::runtime_error("invalid extension '" + directiveArgs[i] + "' in '" + directiveName + "' directive");
            }
        }
    } else if (directiveName == UPLOAD_DIRECTIVE) {
        checkRequirements(UPLOAD_DIRECTIVE_ARGS(directiveArgs.size()), UPLOAD_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::string& uploadStr = directiveArgs.front();
        if (uploadStr.front() != '/') {
            throw std::runtime_error("invalid upload path '" + uploadStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == REDIRECT_DIRECTIVE) {
        checkRequirements(REDIRECT_DIRECTIVE_ARGS(directiveArgs.size()), REDIRECT_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        const std::string& codeStr = directiveArgs.front();
        if (isValidStatusCode(codeStr) == false) {
            throw std::runtime_error("invalid status code '" + codeStr + "' in '" + directiveName + "' directive");
        }
    } else if (directiveName == INCLUDE_DIRECTIVE) {
        checkRequirements(INCLUDE_DIRECTIVE_ARGS(directiveArgs.size()), INCLUDE_DIRECTIVE_POS(currentContext->getName()), directiveName, "directive");
        loadConfiguration(directiveArgs[0], currentContext, depth + 1);
        return ;
    } else if (currentContext->getName()  == TYPES_CONTEXT) {
        std::vector<std::string> mimeType = split(directiveName, WITE_SPACES);
        for (size_t i = 0; i < directiveArgs.size(); ++i) {
            currentContext->addDirective(directiveArgs[i], mimeType);
        }
        return ;
    } else {
        throw std::runtime_error("unknown directive '" + directiveName + "'");
    }
    currentContext->addDirective(directiveName, directiveArgs);
}  

const Context* loadConfiguration(const std::string& configFilePath, Context* configuration, const size_t depth) {
    if (depth > MAX_DEPTH) {
        throw std::runtime_error("max depth reached. (to many nested files).");
    }

    std::ifstream configFile(configFilePath);
    if (configFile.is_open() == false) {
        throw std::runtime_error("open() '" + configFilePath + "' failed : " + strerror(errno) + ".");
    }

    if (configuration == nullptr) {
        configuration =  loadContext(split(MAIN_CONTEXT, WITE_SPACES), nullptr);
    }

    Context* currentContext = configuration;
    std::string token;
    size_t      line = 1;
    char        c = configFile.get();
    while (configFile.good()) {
        switch (c) {
            case '{' :
                try { currentContext = addNewContext(split(token, WITE_SPACES), currentContext); }
                catch (const std::exception& e) {
                    configFile.close();
                    throw std::runtime_error(std::string(e.what()) + " ('" + configFilePath + "' line: " + std::to_string(line) + ").");
                }
                token.clear();
                break;
            case '}' :
                if (currentContext->getParent() == nullptr) {
                    configFile.close();
                    throw std::runtime_error("unexpected '}' ('" + configFilePath + "' line: " + std::to_string(line) + ").");
                }
                currentContext = currentContext->getParent();
                break;
            case ';' :
                try { addNewDirective(split(token, WITE_SPACES), currentContext, depth); }
                catch (const std::exception& e) {
                    configFile.close();
                    throw std::runtime_error(std::string(e.what()) + " ('" + configFilePath + "' line: " + std::to_string(line) + ").");
                }
                token.clear();
                break;
            case '#' : 
                while (configFile.good() && configFile.get() != NEW_LINE);
            case NEW_LINE : 
                ++line;
                break;
            default :
                token += c;
        }
        c = configFile.get();
    }

    if (configFile.eof() == false) {
        configFile.close();
        throw std::runtime_error("read() '" + configFilePath + "' failed : " + strerror(errno) + ".");
    }
    configFile.close();

    if (token.find_first_not_of(WITE_SPACES) != std::string::npos) {
        throw std::runtime_error("unexpected end of file ('" + configFilePath + "' line: " + std::to_string(line) + ").");
    }
    
    return configuration;
}