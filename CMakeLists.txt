cmake_minimum_required(VERSION 3.10)
project(WireframeRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)

find_library(OpenGL_LIBRARY OpenGL)

file(GLOB SOURCES "src/*.cpp")
add_executable(WireframeRenderer ${SOURCES} src/glad.c)

target_include_directories(WireframeRenderer PRIVATE include)

target_include_directories(WireframeRenderer PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(WireframeRenderer PRIVATE glfw ${OpenGL_LIBRARY})
