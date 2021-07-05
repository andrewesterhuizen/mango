cmake_minimum_required(VERSION 3.17)
project(mango)

set(CMAKE_CXX_STANDARD 17)

add_executable(mango
        main.cpp
        lexer.cpp
        token.cpp
        parser.cpp
        ast.cpp
        interpreter/interpreter.cpp
        string_builder.cpp
        types.cpp
        interpreter/call_stack.cpp)