cmake_minimum_required(VERSION 3.22)
project(qzdl)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

list(APPEND EXEC
        miniz/miniz.h
        include/libwad.h
        include/ZDLAboutDialog.h
        include/zdlcommon.h
        include/zdlconf.hpp
        include/ZDLConfiguration.h
        include/ZDLConfigurationManager.h
        include/ZDLFileInfo.h
        include/ZDLFileList.h
        include/ZDLFileListable.h
        include/ZDLFilePane.h
        include/ZDLImportDialog.h
        include/ZDLInterface.h
        include/ZDLIWadList.h
        include/zdlline.hpp
        include/ZDLListable.h
        include/ZDLListEntry.hpp
        include/ZDLListWidget.h
        include/ZDLMainWindow.h
        include/ZDLMapFile.h
        include/ZDLMultiPane.h
        include/ZDLNameInput.h
        include/ZDLNameListable.h
        include/ZDLNullDevice.h
        include/ZDLQSplitter.h
        include/zdlsection.hpp
        include/ZDLSettingsPane.h
        include/ZDLSettingsTab.h
        include/ZDLSourcePortList.h
        include/ZDLWidget.h
        include/ZLibDir.h
        include/ZLibPK3.h
        miniz/miniz.c
        src/libwad.cpp
        src/qzdl.cpp
        src/ZDLAboutDialog.cpp
        src/zdlconf.cpp
        src/ZDLConfiguration.cpp
        src/ZDLConfigurationManager.cpp
        src/ZDLFileInfo.cpp
        src/ZDLFileList.cpp
        src/ZDLFileListable.cpp
        src/ZDLFilePane.cpp
        src/ZDLImportDialog.cpp
        src/ZDLInterface.cpp
        src/ZDLIWadList.cpp
        src/zdlline.cpp
        src/ZDLListable.cpp
        src/ZDLListEntry.cpp
        src/ZDLListWidget.cpp
        src/ZDLMainWindow.cpp
        src/ZDLMapFile.cpp
        src/ZDLMultiPane.cpp
        src/ZDLNameInput.cpp
        src/ZDLNameListable.cpp
        src/ZDLQSplitter.cpp
        src/zdlsection.cpp
        src/ZDLSettingsPane.cpp
        src/ZDLSettingsTab.cpp
        src/ZDLSourcePortList.cpp
        src/ZDLWidget.cpp
        src/ZLibDir.cpp
        src/ZLibPK3.cpp)

list(APPEND INCL
        include
        miniz
        res/XPM)

if (WIN32)
    list(APPEND EXEC
            SimpleWFA/SimpleWFA.h
            include/ZDLFileAssociations.h
            SimpleWFA/SimpleWFA.cpp
            src/ZDLFileAssociations.cpp)

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