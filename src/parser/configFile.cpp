# include <string>
# include <fstream>
# include <iostream>

# include "context.hpp"
# include "parser.hpp"
# include "definitions.hpp"

# ifdef DEBUG_CONFIG_FILE_PARSER
static void debug(const Context* context, const int level) {
    const std::vector<std::string>& args = context->getArgs();
    std::cerr << std::string(level, '\t');
    for (size_t i = 0; i < args.size(); ++i) {
        if (i == 0) {
            std::cerr << "[" << args[i] << "] :: ";
        } else {
            std::cerr << args[i] << " ";
        }
    }
    std::cerr << "(parent: " << (context->getParent() == nullptr ? "null" : context->getParent()->getType()) << ")" << std::endl;
    const std::map<std::string, std::vector<std::string> >& directives = context->getDirectives();
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = directives.begin(); it != directives.end(); ++it) {
        std::cerr << std::string(level + 1, '\t') << "(" << it->first << ") : ";
        for (size_t i = 0; i < it->second.size(); ++i) {
            std::cerr << it->second[i] << " ";
        }
        std::cerr << ";" << std::endl;
    }
    const std::vector<Context*>& children = context->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        debug(children[i], level + 1);
    }
}
# endif

static void panic(std::ifstream& configFile, const std::string& message) {
    std::cerr << "webserv: invalid config file : " << message << "." << std::endl;
    if (configFile.is_open() == true) {
        configFile.close();
    }
    exit(EXIT_FAILURE);
}

void parseConfigFile(const std::string& configFilePath, Context* mainContext) {
    std::ifstream configFile(configFilePath);
    if (configFile.is_open() == false) {
        panic(configFile, strerror(errno));
    }

    std::string token;
    Context* currentContext = mainContext;
    size_t line = 1;
    char c = configFile.get();
    while (configFile.eof() == false) {
        switch (c) {
            case '{' :
                try { currentContext = currentContext->addContext(split(token, WHITE_SPACE)); } catch (const std::invalid_argument& e) {
                    panic(configFile, e.what() + std::string(" : line ") + std::to_string(line));
                }
                token.clear();
                break;
            case ';' :
                try { currentContext->addDirective(split(token, WHITE_SPACE)); } catch (const std::invalid_argument& e) {
                    panic(configFile, e.what() + std::string(" : line ") + std::to_string(line));
                }
                token.clear();
                break;
            case '}' :
                if (currentContext->getType() == MAIN_CONTEXT || token.find_first_not_of(WHITE_SPACE) != std::string::npos) {
                    panic(configFile, "unexpected '}' : line " + std::to_string(line));
                }
                currentContext = currentContext->getParent();
                break;
            case '#' :
                while (configFile.eof() == false && configFile.get() != NEW_LINE);
                ++line;
                break;
            case NEW_LINE :
                ++line;
            default : 
                token += c;
        }
        c = configFile.get();
    }

    if (token.find_first_not_of(WHITE_SPACE) != std::string::npos) {
        panic(configFile, "unexpected end of file");
    }

    configFile.close();

    # ifdef DEBUG_CONFIG_FILE_PARSER
        std::cerr << " DEBUG_CONFIG_FILE_PARSER ----------------------------------------------------- " << std::endl;
        debug(mainContext, 0);
        std::cerr << " ------------------------------------------------------------------------------ " << std::endl;
    # endif
}