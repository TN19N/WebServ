# include "../../include/webserv/IBase.hpp"
# include "../../include/webserv/defines.hpp"

IBase::IBase() 
    : keepAlive(true),
    isChunked(false), 
    state(CREATING),
    contentLength(0) 
{

}

IBase::~IBase() {

}
