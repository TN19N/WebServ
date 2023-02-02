# include "webServ.hpp"
# include "parser.hpp"

Webserv::Webserv(const std::string& configFilePath) 
    : mainContext(new Context())
{
    parseConfigFile(configFilePath, this->mainContext);
    loadMimeTypes(MIME_TYPES_FILE_PATH, this->mimeTypes);
}

void Webserv::run() {
    // TODO
}