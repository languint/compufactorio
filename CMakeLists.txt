cmake_minimum_required(VERSION 3.30)
project(compufactorio)

set(CMAKE_CXX_STANDARD 20)

add_executable(compufactorio main.cpp
        util.h
        lang/ast.h
        lang/parser.h)

