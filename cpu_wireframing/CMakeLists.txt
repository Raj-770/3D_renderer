cmake_minimum_required(VERSION 3.10)
project(cpu_wireframe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")


add_subdirectory(dependencies/minifb)

file(GLOB SOURCES "src/*.cpp")
add_executable(cpu_wireframe ${SOURCES})

target_link_libraries(cpu_wireframe minifb)
target_include_directories(cpu_wireframe PRIVATE include dependencies/minifb/include)

