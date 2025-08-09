cmake_minimum_required(VERSION 3.10)
project(cpu_wireframe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)


file(GLOB SOURCES "src/*.cpp")
add_executable(cpu_wireframe ${SOURCES})

target_include_directories(cpu_wireframe PRIVATE include)

