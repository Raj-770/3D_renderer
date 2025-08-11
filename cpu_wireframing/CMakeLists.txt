cmake_minimum_required(VERSION 3.10)
project(render-gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_subdirectory(dependencies/minifb)

# Gather source files except the two main files
file(GLOB CORE_SOURCES "src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/RenderGUIMain.cpp"
    "${CMAKE_SOURCE_DIR}/src/RenderFileMain.cpp"
)

# Executable: render-gui
add_executable(render-gui src/RenderGUIMain.cpp ${CORE_SOURCES})
target_link_libraries(render-gui minifb)
target_include_directories(render-gui PRIVATE include dependencies/minifb/include)

# Executable: render-to-file
add_executable(render-to-file src/RenderFileMain.cpp ${CORE_SOURCES})
target_link_libraries(render-to-file minifb)
target_include_directories(render-to-file PRIVATE include dependencies/stb_image_write)
