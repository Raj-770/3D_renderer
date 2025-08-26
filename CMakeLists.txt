cmake_minimum_required(VERSION 3.10)
project(render-gui)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


file(GLOB CORE_SOURCES "cpu_wireframing/src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/cpu_wireframing/src/RenderGUIMain.cpp"
    "${CMAKE_SOURCE_DIR}/cpu_wireframing/src/RenderFileMain.cpp"
)


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


add_executable(render-to-file cpu_wireframing/src/RenderFileMain.cpp cpu_wireframing/include/WireframeApp.hpp ${CORE_SOURCES})
target_link_libraries(render-to-file
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
target_include_directories(render-to-file PRIVATE cpu_wireframing/include)
