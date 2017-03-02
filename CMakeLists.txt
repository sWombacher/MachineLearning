#set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_C_COMPILER "clang")


project(machineLearning)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -fconcepts")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
