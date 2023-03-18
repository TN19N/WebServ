# include "webserv/Ibase.hpp"
# include "webserv/defines.hpp"

IBase::IBase() 
    : keepAlive(true),
    isChunked(false), 
    state(CREATING),
    contentLength(0) 
{

}

IBase::~IBase() {

}