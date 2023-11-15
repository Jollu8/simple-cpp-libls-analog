cmake_minimum_required(VERSION 3.23)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

include_directories(../src)

add_executable(server server.cpp ../src/TCPServer.cpp ../src/TCPClient.cpp)
