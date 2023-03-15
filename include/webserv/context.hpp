# ifndef CONTEXT_HPP
# define CONTEXT_HPP

# include <string>
# include <vector>
# include <map>
# include <sys/socket.h>

# define CLOSE_CONNECTION   false
# define KEEP_ALIVE         true

enum { READING_REQUEST, SENDING_RESPONSE, READING_RESPONSE, SENDING_REQUEST, WRITE_BODY_TO_CGI
		UPLOADING_FILE, READING_HEADERS, READING_BODY, BODY_READY, WRITE_END, READ_END } ;

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