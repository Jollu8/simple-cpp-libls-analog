
cmake_minimum_required(VERSION 3.26)
project(mDNS)

add_executable(runner
        #test/Runner.cpp
        #test/QueryTest.cpp
        src/Query.cpp
        #test/CacheTest.cpp
        src/Cache.cpp
        #test/ResponseTest.cpp
        src/Response.cpp
        #test/AnswerTest.cpp
        src/Answer.cpp
        #test/mDNSResolverTest.cpp
        src/mDNSResolver.cpp
        src/Constants.h
        fixtures/IPAddress.h
        ../dns-server/src/data.cpp

        #mocks/WiFiUdp.cpp
        #mocks/IPAddress.cpp
        #mocks/Arduino.cpp
)

target_include_directories(runner PUBLIC
        src
        mocks
)

target_compile_definitions(runner PUBLIC TEST)

add_custom_target(test_runner
        COMMAND $<TARGET_FILE:runner>
        DEPENDS runner
)

add_custom_target(clean_runner
        COMMAND rm runner
)

add_custom_target(debug_runner
        COMMAND gdb runner
)

add_custom_target(ci_runner
        COMMAND CXX=g++-5 make runner
)
