# ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <string>
# include <vector>
# include <map>

# include "context.hpp"
# include "definitions.hpp"

class Context {
    private:
        std::vector<std::string>                          args;
        std::map<std::string, std::vector<std::string> >  directives;
        std::vector<Context*>                             children;
        Context*                                          parent;
    public:
        Context();
        Context(const std::vector<std::string>& args, Context* parent);

        Context* addContext(const std::vector<std::string>& args);
        void addDirective(const std::vector<std::string>& args);

        const std::vector<std::string>&                         getArgs(void)       const;
        const std::string&                                      getType(void)       const;
        const std::map<std::string, std::vector<std::string> >& getDirectives(void) const;
        const std::vector<Context*>&                            getChildren(void)   const;
        const Context*                                          getParent(void)     const;
        Context*                                                getParent(void)          ;
};

# endif