cmake_minimum_required(VERSION 3.30)
project(compufactorio)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/tests/*.h")

# Generate a header file with all tests.
file(WRITE "all_tests.h" "")
file(APPEND "all_tests.h" "#pragma once\n")
foreach(HEADER ${HEADERS})
    file(APPEND "all_tests.h" "#include \"${HEADER}\"\n")
endforeach()

add_executable(compufactorio main.cpp
        util.h
        lang/ast.h
        lang/parser.h
        tests.h
        all_tests.h
)

