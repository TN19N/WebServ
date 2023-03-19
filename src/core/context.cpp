# include <string>
# include <vector>
# include <map>
# include <sys/socket.h>

# include "../../include/webserv/context.hpp"
# include "../../include/webserv/configuration.hpp"
# include "../../include/webserv/core.hpp"

# define WHITE_SPACE " \t\r\n\v\f"

// * Constructor *********************************************************************************************************************
Context::Context(const std::string& name, const std::vector<std::string>& args, Context* parent)
    : name(name),
    args(args),
    parent(parent)
{
    if (this->getName() == HTTP_CONTEXT) {
        this->addDirective(INDEX_DIRECTIVE, CORE::split(DEFAULT_INDEX, WHITE_SPACE));
        this->addDirective(ROOT_DIRECTIVE, CORE::split(DEFAULT_ROOT, WHITE_SPACE));
        this->addDirective(SIZE_DIRECTIVE, CORE::split(DEFAULT_SIZE, WHITE_SPACE));
        this->addDirective(METHOD_DIRECTIVE, CORE::split(DEFAULT_METHOD, WHITE_SPACE));
        this->addDirective(AUTOINDEX_DIRECTIVE, CORE::split(DEFAULT_AUTOINDEX, WHITE_SPACE));
    } else if (this->getName() == SERVER_CONTEXT) {
        this->addDirective(PORT_DIRECTIVE, CORE::split(DEFAULT_PORT, WHITE_SPACE));
        this->addDirective(HOST_DIRECTIVE, CORE::split(DEFAULT_HOST, WHITE_SPACE));
    }

    if (parent != nullptr) {
        this->directives.insert(parent->getDirectives().begin(), parent->getDirectives().end());
    }
}
// **********************************************************************************************************************************

// * Getters ************************************************************************************************************************
const std::string& Context::getName() const {
    return this->name;
}

const std::vector<std::string>& Context::getArgs() const {
    return this->args;
}

const Context::Directives& Context::getDirectives() const {
    return this->directives;
}

const std::vector<std::string>& Context::getDirective(const std::string& name) const {
    return this->directives.at(name);
}

const std::vector<Context*>& Context::getChildren() const {
    return this->children;
}

const Context* Context::getParent() const {
    return this->parent;
}

Context* Context::getParent() {
    return this->parent;
}
// **********************************************************************************************************************************

// * Setters ************************************************************************************************************************
void Context::addDirective(const std::string& name, const std::vector<std::string>& args) {
    this->directives[name] = args;
}

void Context::addChild(Context* child) {
    this->children.push_back(child);
}
// **********************************************************************************************************************************

// * Destructor *********************************************************************************************************************
Context::~Context() {
    for (std::vector<Context*>::iterator it = this->children.begin(); it != this->children.end(); ++it) {
        delete *it;
    }
}
// **********************************************************************************************************************************