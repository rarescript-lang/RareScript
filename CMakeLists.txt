cmake_minimum_required(VERSION 4.3)
project(rpgen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_library(rarescript_lib STATIC
        rarescript/lexer/lexer.cpp
        rarescript/parser/parser.cpp
        rarescript/parser/expr/expr.cpp
)

target_include_directories(rarescript_lib
        PUBLIC
        rarescript/lexer
        rarescript/parser
)

add_executable(rpgen
        main.cpp
)

target_link_libraries(rpgen
        PRIVATE
        rarescript_lib
)

add_executable(rpgen_tests
        tests/lexer/lexer.cpp
)

target_link_libraries(rpgen_tests
        PRIVATE
        Catch2::Catch2WithMain
        rarescript_lib
)

catch_discover_tests(rpgen_tests)

if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(rpgen PRIVATE stdc++exp)
    target_link_libraries(rpgen_tests PRIVATE stdc++exp)
endif ()