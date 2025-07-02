cmake_minimum_required(VERSION 3.10)
project(JSON_Parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)  

add_library(json_parser
    src/json.cpp
    src/json_value.cpp
    src/parser.cpp
    src/tokenizer.cpp
)

target_include_directories(json_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTS)
    add_executable(json_tests
        tests/CompleteTestSuite.cpp
        tests/test_main.cpp
    )
    target_link_libraries(json_tests PRIVATE json_parser)
    target_include_directories(json_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    )

    enable_testing()
    add_test(NAME json_tests COMMAND json_tests)
endif()
