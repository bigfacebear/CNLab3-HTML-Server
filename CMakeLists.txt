cmake_minimum_required(VERSION 3.7)
project(WebServer)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES src/main.cpp include/Request.h include/Response.h)
add_executable(WebServer ${SOURCE_FILES})