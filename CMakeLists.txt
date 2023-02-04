cmake_minimum_required(VERSION 3.8)
project(qzdl
        DESCRIPTION "Launcher for ZDoom based Doom engine source ports"
        HOMEPAGE_URL "https://github.com/spacebub/qzdl"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(QT5 "Build with Qt5 instead of Qt6" OFF)
option(MSVC_STATIC "Enable static linking for msvc builds" ON)

if(MSVC)
    if (MSVC_STATIC)
        add_compile_options("/W4" "/MP" "/GS" "/MTd" "$<$<CONFIG:RELEASE>:/O1;/MT;/GL;/Gw;/Gy>")
    else()
        add_compile_options("/W4" "/MP" "/GS" "$<$<CONFIG:RELEASE>:/O1;/GL;/Gw;/Gy>")
    endif()

    add_link_options("$<$<CONFIG:RELEASE>:/LTCG>")
else()
    add_compile_options("-Wall" "-Wextra" "-Werror" "-fstack-protector-strong" "$<$<CONFIG:RELEASE>:-O3>")
endif()

if (QT5)
    find_package(Qt5 REQUIRED COMPONENTS
            Core)
else()
    find_package(Qt6 REQUIRED COMPONENTS
            Core)
endif()

add_subdirectory(src)