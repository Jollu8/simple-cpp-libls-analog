cmake_minimum_required(VERSION 3.23)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

find_package(Boost REQUIRED COMPONENTS system)

include_directories(../src)

add_executable(client client.cpp client_asio.cpp ../src/TCPClient.cpp ../src/TCPServer.cpp)
target_link_libraries(client Boost::system )
