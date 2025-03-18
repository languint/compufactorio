cmake_minimum_required(VERSION 3.30)
project(compufactorio)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(ZLIB)

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
        tests/parser_test.h
        lang/type_checker.h
        tests/type_checker_test.h
        blueprint/blueprint.h
        base64.h
        blueprint/types.h
        blueprint/tiler.h
        tests/blueprint_test.h
)

target_link_libraries(compufactorio PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(compufactorio PRIVATE z)