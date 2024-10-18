cmake_minimum_required(VERSION 3.16.0)

project(jjson)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(include_dir ${PROJECT_SOURCE_DIR}/include 
                ${PROJECT_SOURCE_DIR}/include/jjson)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)