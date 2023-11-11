CMAKE_MINIMUM_REQUIRED(VERSION 3.23)
PROJECT(http VERSION 0.0.1)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# Предварительно скомпилированные заголовки
FILE(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/http/*.h )
FILE(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )

ADD_LIBRARY( http_lib ${SOURCE_FILES} ${HEADER_FILES})

#target_precompile_headers(YourLibraryName PUBLIC "path_to_your_pch")

# Подключение библиотек
#find_package(http_lib REQUIRED)
#target_link_libraries(YourLibraryName PRIVATE LibName)

# Подключение тестов
#enable_testing()
#add_executable(YourTestName path_to_your_test_file)
#target_link_libraries(YourTestName PRIVATE YourLibraryName)
#add_test(NAME YourTestName COMMAND YourTestName)
