# ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <string>
# include <vector>
# include <map>

class Context {
    private:
        const std::vector<std::string>                          args;
        std::map<const std::string, std::vector<std::string> >  directives;
        std::vector<Context*>                                   children;
        Context*                                                parent;
    public:
        Context(const std::vector<std::string>& args, Context* parent);

        // setters
        void addDirective(const std::string& key, const std::vector<std::string>& value);
        void addChild(Context* child);

        // getters
        const std::vector<std::string>&                                 getArgs()       const;
        const std::string&                                              getName()       const;
        const std::map<const std::string, std::vector<std::string> >&   getDirectives() const;
        const std::vector<Context*>&                                    getChildren()   const;
        const Context*                                                  getParent()     const;
        Context*                                                        getParent();

        ~Context();
};

# endif