cmake_minimum_required(VERSION 3.10)
project(WireframeRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Link to the system OpenGL framework on macOS
find_library(OpenGL_LIBRARY OpenGL)

add_executable(WireframeRenderer src/main.cpp)
target_include_directories(WireframeRenderer PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(WireframeRenderer PRIVATE glfw ${OpenGL_LIBRARY})
