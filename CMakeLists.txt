cmake_minimum_required(VERSION 3.16)
project(Fork_Knight C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "src/*.c")

add_executable(fork_knight ${SOURCE_FILES})
target_include_directories(fork_knight PRIVATE headers)
