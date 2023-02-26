# include <string>
# include <vector>

# include "webserv/context.hpp"

Context::Context(const std::vector<std::string>& args, Context* parent) 
    : args(args), 
    parent(parent)
{
    if (parent != nullptr) {
        this->directives = parent->getDirectives();
    }
}

void Context::addDirective(const std::string& key, const std::vector<std::string>& value) {
    this->directives[key] = value;
}

void Context::addChild(Context* child) {
    this->children.push_back(child);
}

const std::vector<std::string>& Context::getArgs() const {
    return this->args;
}

const std::string& Context::getName() const {
    return this->args.front();
}

const std::map<const std::string, std::vector<std::string> >& Context::getDirectives() const {
    return this->directives;
}

const std::vector<std::string>& Context::getDirective(const std::string& key) const {
    return this->directives.at(key);
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

Context::~Context() {
    for (std::vector<Context*>::const_iterator it = this->children.begin(); it != this->children.end(); ++it) {
        delete (*it);
    }
}