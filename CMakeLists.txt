CMAKE_MINIMUM_REQUIRED(VERSION 3.19 )
PROJECT(CPP)


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-elide-constructors -fno-implement-inlines -fno-implicit-inline-templates -fno-default-inline -fno-inline")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -std=c++20 -Wall -Wno-reorder -Werror=return-type")
endif()

add_subdirectory(tcp/src)
add_executable(CPP runs.cpp)