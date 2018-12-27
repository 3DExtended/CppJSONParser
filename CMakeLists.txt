cmake_minimum_required(VERSION 3.1)
project(CppJSONParser CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hh" "include/*.h" "include/*.hpp")

if(MSVC)
    set(CPPJSONPARSER_LINK_TYPE STATIC)
else()
    set(CPPJSONPARSER_LINK_TYPE SHARED)
endif()

add_library(cppJSONParser ${CPPJSONPARSER_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES})

target_compile_options( cppJSONParser PUBLIC -std=c++11)


target_include_directories(cppJSONParser PUBLIC
    include/
)

add_subdirectory("example")
