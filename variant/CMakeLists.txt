
cmake_minimum_required(VERSION 3.19)
project(Variant)


set(SRC_LIST
        main.cpp
        variant.h
        readme.md
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
)