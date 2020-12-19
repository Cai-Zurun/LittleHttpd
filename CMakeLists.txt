cmake_minimum_required(VERSION 3.16)
project(littleHttpd2)

set(CMAKE_CXX_STANDARD 14)

add_executable(littleHttpd2 main.cpp socket/socket.cpp server/server.cpp httprequest/httprequest.cpp httpresponse/httpresponse.cpp)