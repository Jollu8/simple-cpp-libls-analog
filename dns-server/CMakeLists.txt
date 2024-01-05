cmake_minimum_required(VERSION 3.26)

project("dns-lib")

find_package(Boost REQUIRED system)

#find_package(GTest CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

#enable_testing()


add_executable("dns-lib"
        include/dns/buffer.h
        include/dns/data.h
        include/dns/error.h
        include/dns/lookup.h
        include/dns/recursive_lookup.h
        include/dns/resolver.h
        include/dns/responder.h
        include/dns/static_lookup.h
        include/dns/udp.h

        src/data.cpp
        src/error.cpp
        src/lookup.cpp
        src/recursive_lookup.cpp
        src/resolver.cpp
        src/static_lookup.cpp
)
set_property(TARGET "dns-lib" PROPERTY CXX_STANDARD 20)
target_include_directories("dns-lib" PUBLIC include)
target_include_directories("dns-lib" PRIVATE include/dns)

target_link_libraries("dns-lib" PRIVATE Boost::system)


#add_executable(dns_lib_test
#        test/test_byte_output.cpp
#        test/test_input.cpp
#        test/test_pointer.cpp
#        test/test_resolution.cpp
#        test/test_responder.cpp
#        test/test_standard_queries.cpp
#        test/test_unreliable_server.cpp
#        test/test_wildcard.cpp
#)
#
#set_property(TARGET dns_lib_test PROPERTY CXX_STANDARD 20)
#target_link_libraries(dns_lib_test PRIVATE GTest::gtest_main GTest::gmock_main Boost::asio "dns-lib")

#add_executable(dns_example_basic examples/udp_basic.cpp)
#set_property(TARGET dns_example_basic PROPERTY CXX_STANDARD 20)
#target_link_libraries(dns_example_basic PRIVATE argparse::argparse "dns-lib" Boost::asio)

#add_executable(dns_example_recursive examples/udp_recursive.cpp)
#set_property(TARGET dns_example_recursive PROPERTY CXX_STANDARD 20)
#target_link_libraries(dns_example_recursive PRIVATE bighorn asio::asio)
#
#include(GoogleTest)
#gtest_discover_tests(bighorn_test)