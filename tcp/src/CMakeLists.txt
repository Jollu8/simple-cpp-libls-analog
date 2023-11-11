cmake_minimum_required(VERSION 3.23)
project(SimpleNetwork VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(SimpleNetwork SHARED TCPServer.cpp TCPClient.cpp)
add_library(SimpleNetworkStatic STATIC TCPServer.cpp TCPClient.cpp)

target_include_directories(SimpleNetwork PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SimpleNetworkStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(SimpleNetwork PROPERTIES OUTPUT_NAME "SimpleNetwork")
set_target_properties(SimpleNetworkStatic PROPERTIES OUTPUT_NAME "SimpleNetwork")
