cmake_minimum_required(VERSION 3.22)
project(qzdl
        DESCRIPTION "Launcher for ZDoom based Doom engine source ports"
        HOMEPAGE_URL "https://github.com/spacebub/qzdl"
        LANGUAGES C CXX)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(QT5 "Build with Qt5 instead of Qt6" OFF)

if (QT5)
    find_package(Qt5 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)
else()
    find_package(Qt6 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)
endif()

list(APPEND EXEC
        external/miniz/miniz.h
        external/miniz/miniz.c
        src/config.h
        src/libwad.h
        src/libwad.cpp
        src/qzdl.cpp
        src/ZDLAboutDialog.h
        src/ZDLAboutDialog.cpp
        src/zdlcommon.h
        src/ZDLConfiguration.h
        src/ZDLConfiguration.cpp
        src/zdlconf.hpp
        src/zdlconf.cpp
        src/ZDLConfigurationManager.h
        src/ZDLConfigurationManager.cpp
        src/ZDLFileInfo.h
        src/ZDLFileInfo.cpp
        src/ZDLFileList.h
        src/ZDLFileList.cpp
        src/ZDLFileListable.h
        src/ZDLFileListable.cpp
        src/ZDLFilePane.h
        src/ZDLFilePane.cpp
        src/ZDLImportDialog.h
        src/ZDLImportDialog.cpp
        src/ZDLInterface.h
        src/ZDLInterface.cpp
        src/ZDLIWadList.h
        src/ZDLIWadList.cpp
        src/zdlline.hpp
        src/zdlline.cpp
        src/ZDLListable.h
        src/ZDLListable.cpp
        src/ZDLListEntry.hpp
        src/ZDLListEntry.cpp
        src/ZDLListWidget.h
        src/ZDLListWidget.cpp
        src/ZDLMainWindow.h
        src/ZDLMainWindow.cpp
        src/ZDLMapFile.h
        src/ZDLMapFile.cpp
        src/ZDLMultiPane.h
        src/ZDLMultiPane.cpp
        src/ZDLNameInput.h
        src/ZDLNameInput.cpp
        src/ZDLNameListable.h
        src/ZDLNameListable.cpp
        src/ZDLNullDevice.h
        src/ZDLQSplitter.h
        src/ZDLQSplitter.cpp
        src/zdlsection.hpp
        src/zdlsection.cpp
        src/ZDLSettingsPane.h
        src/ZDLSettingsPane.cpp
        src/ZDLSettingsTab.h
        src/ZDLSettingsTab.cpp
        src/ZDLSourcePortList.h
        src/ZDLSourcePortList.cpp
        src/ZDLWidget.h
        src/ZDLWidget.cpp
        src/ZLibDir.h
        src/ZLibDir.cpp
        src/ZLibPK3.h
        src/ZLibPK3.cpp)

list(APPEND INCL
        include
        external/miniz
        external/rapidjson
        res/XPM)

if (WIN32)
    list(APPEND EXEC
            src/win32/SimpleWFA.h
            src/win32/SimpleWFA.cpp
            src/win32/ZDLFileAssociations.h
            src/win32/ZDLFileAssociations.cpp)

    list(APPEND INCL
            res/Win32
            SimpleWFA)
endif ()

add_executable(qzdl ${EXEC})

target_link_libraries(qzdl PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets)

target_include_directories(qzdl PRIVATE ${INCL})