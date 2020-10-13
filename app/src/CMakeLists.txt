cmake_minimum_required(VERSION 3.18.2)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB SRC *.cpp)

add_library(app ${SRC})

target_link_libraries(app Qt5::Widgets Qt5::Core)
