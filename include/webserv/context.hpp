# ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <string>
# include <vector>
# include <map>
# include <sys/socket.h>

class Context {
    private:
        const std::string                                   name;
        const std::vector<std::string>                      args;
        std::map<std::string, std::vector<std::string> >    directives;
        std::vector<Context*>                               children;
        Context*                                            parent;
    public:
        Context(const std::string& name, const std::vector<std::string>& args = std::vector<std::string>(), Context* parent = nullptr);

        const std::string&                                      getName() const;
        const std::vector<std::string>&                         getArgs() const;
        const std::map<std::string, std::vector<std::string> >& getDirectives() const;
        const std::vector<std::string>&                         getDirective(const std::string& name) const;
        const std::vector<Context*>&                            getChildren() const;
        const Context*                                          getParent() const;
        Context*                                                getParent();

        void    addDirective(const std::string& name, const std::vector<std::string>& args);
        void    addChild(Context* child);

        ~Context();
};

# endif