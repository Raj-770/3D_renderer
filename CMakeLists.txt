cmake_minimum_required(VERSION 3.10)
project(render-gui)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# add_subdirectory(cpu_wireframing/dependencies/minifb)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


# Gather source files except the two main files
file(GLOB CORE_SOURCES "cpu_wireframing/src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/cpu_wireframing/src/RenderGUIMain.cpp"
    "${CMAKE_SOURCE_DIR}/cpu_wireframing/src/RenderFileMain.cpp"
)


# Executable: render-gui
add_executable(render-gui
  cpu_wireframing/src/RenderGUIMain.cpp
  cpu_wireframing/include/WireframeApp.hpp
  ${CORE_SOURCES}
)
target_link_libraries(render-gui
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
target_include_directories(render-gui PRIVATE cpu_wireframing/include cpu_wireframing/dependencies/minifb/include)


# Executable: render-to-file
add_executable(render-to-file cpu_wireframing/src/RenderFileMain.cpp cpu_wireframing/include/WireframeApp.hpp ${CORE_SOURCES})
target_link_libraries(render-to-file
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
target_include_directories(render-to-file PRIVATE cpu_wireframing/include)
