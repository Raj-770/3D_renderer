cmake_minimum_required(VERSION 3.10)
project(gpu_wireframe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)

find_library(OpenGL_LIBRARY OpenGL)

file(GLOB SOURCES "src/*.cpp")
add_executable(gpu_wireframe ${SOURCES} src/glad.c)

target_include_directories(gpu_wireframe PRIVATE include)

target_link_libraries(gpu_wireframe PRIVATE glfw ${OpenGL_LIBRARY})
