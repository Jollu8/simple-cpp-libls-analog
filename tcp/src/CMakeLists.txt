CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
PROJECT(SimpleNetwork VERSION 1.0.1)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB HEADER TCPClient.h TCPServer.cpp) # Добавьте здесь Socket.h
FILE(GLOB SOURCE TCPServer.cpp TCPClient.cpp)
ADD_LIBRARY(SimpleNetwork SHARED
        ${HEADER}
        ${SOURCE}
)
ADD_LIBRARY(SimpleNetworkStatic STATIC
        ${HEADER}
        ${SOURCE}

)

TARGET_INCLUDE_DIRECTORIES(SimpleNetwork
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)
TARGET_INCLUDE_DIRECTORIES(SimpleNetworkStatic
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)

if (MSVC)
    TARGET_COMPILE_OPTIONS(SimpleNetwork PRIVATE
            /W4
    )
    target_compile_options(SimpleNetworkStatic PRIVATE
            /W4
    )

else ()
    target_compile_options(SimpleNetwork PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(SimpleNetworkStatic PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif ()

SET_TARGET_PROPERTIES(SimpleNetwork PROPERTIES OUTPUT_NAME "SimpleNetwork")
SET_TARGET_PROPERTIES(SimpleNetworkStatic PROPERTIES OUTPUT_NAME "SimpleNetwork")
