cmake_minimum_required(VERSION 3.24)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/webserv)

add_executable(webserv
        include/webserv/client.hpp
        include/webserv/configuration.hpp
        include/webserv/context.hpp
        include/webserv/core.hpp
        include/webserv/defines.hpp
        include/webserv/http.hpp
        include/webserv/IBase.hpp
        include/webserv/request.hpp
        include/webserv/response.hpp
        include/webserv/webserv.hpp
        src/core/context.cpp
        src/core/loadConfiguration.cpp
        src/core/luncher.cpp
        src/core/tools.cpp
        src/core/webserv.cpp
        src/http/baseParser.cpp
        src/http/blockMatchAlgorithm.cpp
        src/http/cgiExecutor.cpp
        src/http/client.cpp
        src/http/connectionHandler.cpp
        src/http/deleteMethodHundler.cpp
        src/http/getMethodHandler.cpp
        src/http/IBase.cpp
        src/http/postMethodHundler.cpp
        src/http/readBodyFromBuffer.cpp
        src/http/request.cpp
        src/http/requestHandler.cpp
        src/http/response.cpp
        src/http/responseHandler.cpp
        src/http/tools.cpp)
