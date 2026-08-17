cmake_minimum_required(VERSION 4.3)
project(rpgen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_library(rarescript_lib STATIC
        rarescript/lexer/lexer.cpp
        rarescript/parser/parser.cpp
        rarescript/parser/ast/segment.cpp
        rarescript/parser/expr/expr.cpp
        rarescript/parser/expr/binary_expr.cpp
        rarescript/parser/expr/unary_expr.cpp
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

if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(rpgen PRIVATE stdc++exp)
    target_link_libraries(rpgen_tests PRIVATE stdc++exp)
endif ()