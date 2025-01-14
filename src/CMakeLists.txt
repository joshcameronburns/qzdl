set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (QT5)
    find_package(Qt5
            REQUIRED COMPONENTS
            Core
            Gui
            Widgets)
else ()
    find_package(Qt6
            REQUIRED COMPONENTS
            Core
            Gui
            Widgets)
endif ()

add_executable(qzdl
        external/miniz/miniz.h
        external/miniz/miniz.c
        libwad.cpp
        libwad.h
        qzdl.cpp
        ZDLAboutDialog.cpp
        ZDLAboutDialog.h
        zdlcommon.h
        zdlconf.cpp
        zdlconf.hpp
        ZDLConfiguration.cpp
        ZDLConfiguration.h
        ZDLConfigurationManager.cpp
        ZDLConfigurationManager.h
        ZDLFileInfo.cpp
        ZDLFileInfo.h
        ZDLFileList.cpp
        ZDLFileList.h
        ZDLFileListable.cpp
        ZDLFileListable.h
        ZDLFilePane.cpp
        ZDLFilePane.h
        ZDLImportDialog.cpp
        ZDLImportDialog.h
        ZDLInterface.cpp
        ZDLInterface.h
        ZDLIWadList.cpp
        ZDLIWadList.h
        zdlline.cpp
        zdlline.hpp
        ZDLListable.cpp
        ZDLListable.h
        ZDLListEntry.cpp
        ZDLListEntry.hpp
        ZDLListWidget.cpp
        ZDLListWidget.h
        ZDLMainWindow.cpp
        ZDLMainWindow.h
        ZDLMapFile.cpp
        ZDLMapFile.h
        ZDLMultiPane.cpp
        ZDLMultiPane.h
        ZDLNameInput.cpp
        ZDLNameInput.h
        ZDLNameListable.cpp
        ZDLNameListable.h
        ZDLNullDevice.h
        ZDLQSplitter.cpp
        ZDLQSplitter.h
        zdlsection.cpp
        zdlsection.hpp
        ZDLSettingsPane.cpp
        ZDLSettingsPane.h
        ZDLSettingsTab.cpp
        ZDLSettingsTab.h
        ZDLSourcePortList.cpp
        ZDLSourcePortList.h
        ZDLWidget.cpp
        ZDLWidget.h
        ZLibDir.cpp
        ZLibDir.h
        ZLibPK3.cpp
        ZLibPK3.h)

target_link_libraries(qzdl
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets)

target_include_directories(qzdl
        PRIVATE
        external/miniz
        xpm)

if (WIN32)
    target_sources(qzdl
            PRIVATE
            resources/Win32/resource.h
            resources/Win32/qzdl.rc
            SimpleWFA.h
            SimpleWFA.cpp
            ZDLFileAssociations.h
            ZDLFileAssociations.cpp)
endif ()

set_target_properties(qzdl
        PROPERTIES
        OUTPUT_NAME ZDL)