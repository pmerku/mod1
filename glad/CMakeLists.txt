# cmake minimum version and project name
cmake_minimum_required(VERSION 3.16)
project(glad C)

# set compiler flags and standard
set(CMAKE_C_STANDARD 99)
set(FLAGS "-O3 -Wall -Wextra -Werror")

# static library sources
file(GLOB_RECURSE HEADERS "**.h")
file(GLOB_RECURSE SOURCES "**.c")

include_directories(.)
add_library(glad STATIC ${SOURCES} ${HEADERS})