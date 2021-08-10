# cmake minimum version and project name
cmake_minimum_required(VERSION 3.16)
project(mod1 CXX)

# set compiler flags and standard
set(CMAKE_CXX_STANDARD 17)
set(FLAGS "-O3 -Wall -Wextra -Werror -pedantic-errors")

# set debug flags on debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG_THROW=1")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG_THROW=1")
endif()

# set cmake output directory
set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/build)

# source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/**.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/**.hpp)

# set the include directories
include_directories(src/includes)
include_directories(src)

# add external libraries
add_subdirectory(glfw)
include_directories(glfw/include)

add_subdirectory(glm)
include_directories(glm/glm)

add_subdirectory(glad)
include_directories(glad)

# copy resources into build folder
file(COPY resources DESTINATION ${dir}/build)

# create executable and link the library
add_executable(mod1 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(mod1 glfw glm glad)