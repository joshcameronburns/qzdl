cmake_minimum_required(VERSION 3.22)
project(qzdl
        DESCRIPTION "Launcher for ZDoom based Doom engine source ports"
        HOMEPAGE_URL "https://github.com/spacebub/qzdl"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(QT5 "Build with Qt5 instead of Qt6" OFF)

if (MSVC)
    add_compile_options(/W4 /MP /MT)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (QT5)
    find_package(Qt5 REQUIRED COMPONENTS
            Core)
else()
    find_package(Qt6 REQUIRED COMPONENTS
            Core)
endif()

add_subdirectory(src)