# include <iostream>
# include <string>
# include <vector>
# include <arpa/inet.h>

# include "context.hpp"
# include "definitions.hpp"
# include "parser.hpp"

Context::Context()
    : parent(nullptr)
{
    this->args.push_back(MAIN_CONTEXT);
}

Context::Context(const std::vector<std::string>& args, Context* parent)
    : args(args),
      directives(parent->directives),
      parent(parent)
{
    if (args.front() == HTTP_CONTEXT) {
        this->directives[INDEX_DIRECTIVE] = split(DEFAULT_INDEX, WHITE_SPACE);
        this->directives[ROOT_DIRECTIVE] = split(DEFAULT_ROOT, WHITE_SPACE);
        this->directives[AUTOINDEX_DIRECTIVE] = split(DEFAULT_AUTOINDEX, WHITE_SPACE);
        this->directives[ALLOWED_METHODS_DIRECTIVE] = split(DEFAULT_ALLOWED_METHODS, WHITE_SPACE);
        this->directives[BODY_SIZE_DIRECTIVE] = split(DEFAULT_BODY_SIZE, WHITE_SPACE);
    } else if (args.front() == SERVER_CONTEXT) {
        this->directives[PORT_DIRECTIVE] = split(DEFAULT_PORT, WHITE_SPACE);
        this->directives[HOST_DIRECTIVE] = split(DEFAULT_HOST, WHITE_SPACE);
        this->directives[NAME_DIRECTIVE] = split(DEFAULT_NAME, WHITE_SPACE);
    }
}

Context* Context::addContext(const std::vector<std::string>& tokens) {
    if (tokens.empty() == true) {
        throw std::invalid_argument("empty context");
    }

    const std::string& contextType = tokens.front();
    const std::vector<std::string> contextArgs(tokens.begin() + 1, tokens.end());

    if (contextType == HTTP_CONTEXT) { // HTTP CONTEXT -----------------------------------------------------------------------
        if (HTTP_CONTEXT_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(HTTP_CONTEXT) + "' context not allowed here");
        }
        if (HTTP_CONTEXT_ARGS(contextArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(HTTP_CONTEXT) + "' context");
        }
    } else if (contextType == SERVER_CONTEXT) { // SERVER CONTEXT -----------------------------------------------------------
        if (SERVER_CONTEXT_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(SERVER_CONTEXT) + "' context not allowed here");
        }
        if (SERVER_CONTEXT_ARGS(contextArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(SERVER_CONTEXT) + "' context");
        }
    } else if (contextType == LOCATION_CONTEXT) { // LOCATION CONTEXT -------------------------------------------------------
        if (LOCATION_CONTEXT_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(LOCATION_CONTEXT) + "' context not allowed here");
        }
        if (LOCATION_CONTEXT_ARGS(contextArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(LOCATION_CONTEXT) + "' context");
        }
    } else {
        throw std::invalid_argument("unknown context '" + contextType + "'");
    }
    this->children.push_back(new Context(tokens, this));
    return this->children.back();
}

void Context::addDirective(const std::vector<std::string>& tokens) {
    if (tokens.empty() == true) {
        throw std::invalid_argument("empty directive");
    }
    
    const std::string& directiveType = tokens.front();
    const std::vector<std::string> directiveArgs(tokens.begin() + 1, tokens.end());

    if (directiveType == PORT_DIRECTIVE) { // PORT DIRECTIVE -----------------------------------------------------------------
        if (PORT_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(PORT_DIRECTIVE) + "' directive not allowed here");
        } else if (PORT_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(PORT_DIRECTIVE) + "' directive");
        }
        char *end = nullptr;
        size_t port = std::strtol(directiveArgs.front().c_str(), &end, 10);
        if (*end != '\0' || port < 0 || port > MAX_PORT || (directiveArgs.front().front() == '0' && directiveArgs.front().size() > 1)) {
            throw std::invalid_argument("invalid argument for '" + std::string(PORT_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == HOST_DIRECTIVE) { // HOST DIRECTIVE ----------------------------------------------------------
        if (HOST_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(HOST_DIRECTIVE) + "' directive not allowed here");
        } else if (HOST_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(HOST_DIRECTIVE) + "' directive");
        }
        struct sockaddr_storage sa;
        if (inet_pton(AF_INET, directiveArgs.front().c_str(), &((struct sockaddr_in *)&sa)->sin_addr) != 1 && \
            inet_pton(AF_INET6, directiveArgs.front().c_str(), &((struct sockaddr_in6 *)&sa)->sin6_addr) != 1) {
            throw std::invalid_argument("invalid argument for '" + std::string(HOST_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == NAME_DIRECTIVE) { // NAME DIRECTIVE ----------------------------------------------------------
        if (NAME_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(NAME_DIRECTIVE) + "' directive not allowed here");
        } else if (NAME_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(NAME_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == INDEX_DIRECTIVE) { // INDEX DIRECTIVE --------------------------------------------------------
        if (INDEX_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(INDEX_DIRECTIVE) + "' directive not allowed here");
        } else if (INDEX_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(INDEX_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == ROOT_DIRECTIVE) { // ROOT DIRECTIVE ----------------------------------------------------------
        if (ROOT_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(ROOT_DIRECTIVE) + "' directive not allowed here");
        } else if (ROOT_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(ROOT_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == AUTOINDEX_DIRECTIVE) { // AUTOINDEX DIRECTIVE ------------------------------------------------
        if (AUTOINDEX_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(AUTOINDEX_DIRECTIVE) + "' directive not allowed here");
        } else if (AUTOINDEX_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(AUTOINDEX_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == ALLOWED_METHODS_DIRECTIVE) { // ALLOWED_METHODS DIRECTIVE ------------------------------------
        if (ALLOWED_METHODS_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(ALLOWED_METHODS_DIRECTIVE) + "' directive not allowed here");
        } else if (ALLOWED_METHODS_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(ALLOWED_METHODS_DIRECTIVE) + "' directive");
        } 
    } else if (directiveType == UPLOAD_PATH_DIRECTIVE) { // UPLOAD_PATH DIRECTIVE --------------------------------------------
        if (UPLOAD_PATH_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(UPLOAD_PATH_DIRECTIVE) + "' directive not allowed here");
        } else if (UPLOAD_PATH_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(UPLOAD_PATH_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == CGI_DIRECTIVE) { // CGI DIRECTIVE ------------------------------------------------------------
        if (CGI_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(CGI_DIRECTIVE) + "' directive not allowed here");
        } else if (CGI_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(CGI_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == REDIRECT_DIRECTIVE) { // REDIRECT DIRECTIVE --------------------------------------------------
        if (REDIRECT_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(REDIRECT_DIRECTIVE) + "' directive not allowed here");
        } else if (REDIRECT_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(REDIRECT_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == BODY_SIZE_DIRECTIVE) { // BODY_SIZE DIRECTIVE ------------------------------------------------
        if (BODY_SIZE_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(BODY_SIZE_DIRECTIVE) + "' directive not allowed here");
        } else if (BODY_SIZE_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(BODY_SIZE_DIRECTIVE) + "' directive");
        }
    } else if (directiveType == ERROR_PAGE_DIRECTIVE) { // ERROR_PAGE DIRECTIVE ----------------------------------------------
        if (ERROR_PAGE_DIRECTIVE_POS(this->getType())) {
            throw std::invalid_argument("'" + std::string(ERROR_PAGE_DIRECTIVE) + "' directive not allowed here");
        } else if (ERROR_PAGE_DIRECTIVE_ARGS(directiveArgs.size())) {
            throw std::invalid_argument("invalid number of arguments for '" + std::string(ERROR_PAGE_DIRECTIVE) + "' directive");
        }
        for (size_t i = 0; i < directiveArgs.size(); i += 2) {
            this->directives[directiveArgs[i]] = std::vector<std::string> (directiveArgs.begin() + i + 1, directiveArgs.begin() + i + 2);
        }
        return;
    } else {
        throw std::invalid_argument("unknown directive '" + directiveType + "'");
    }
    this->directives[directiveType] = directiveArgs;
}

const std::vector<std::string>& Context::getArgs(void) const {
    return this->args;
}

const std::string& Context::getType(void) const {
    return this->args.front();
}

const std::map<std::string, std::vector<std::string> >& Context::getDirectives(void) const {
    return this->directives;
}

const std::vector<Context*>& Context::getChildren(void) const {
    return this->children;
}

const Context* Context::getParent(void) const {
    return this->parent;
}

Context* Context::getParent(void) {
    return this->parent;
}