# ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <string>
# include <vector>
# include <map>
# include <sys/socket.h>

#include "webserv/defines.hpp"

class Context {
	public:
		typedef std::map<std::string, std::vector<std::string> > Directives;
    private:
        const std::string             		name;
        const std::vector<std::string>		args;
        Context::Directives    				directives;
        std::vector<Context*>               children;
        Context*                            parent;
    public:
        Context(const std::string& name, const std::vector<std::string>& args = std::vector<std::string>(), Context* parent = NULL);

        const std::string&                                      getName() const;
        const std::vector<std::string>&                         getArgs() const;
        const Context::Directives& getDirectives() const;
        const std::vector<std::string>&                         getDirective(const std::string& name) const;
        const std::vector<Context*>&                            getChildren() const;
        const Context*                                          getParent() const;
        Context*                                                getParent();

        void    addDirective(const std::string& name, const std::vector<std::string>& args);
        void    addChild(Context* child);

        ~Context();
};

# endif