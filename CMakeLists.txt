cmake_minimum_required(VERSION 3.30)
project(sfml)

set(CMAKE_CXX_STANDARD 20)

add_executable(sfml main.cpp
        util.h
        lang/ast.h
        lang/types.h
        toml.h
        lang/parser.h
        sfml.h
        lang/transpiler.h)
