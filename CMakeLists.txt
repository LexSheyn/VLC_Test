cmake_minimum_required(VERSION 3.14)

project(AVCodec_Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets Network)
find_library(vlc libvlc)

file(GLOB_RECURSE PROJECT_HEADERS "*.h")
file(GLOB_RECURSE PROJECT_SOURCES "*.cpp")

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Network
    vlc
)
