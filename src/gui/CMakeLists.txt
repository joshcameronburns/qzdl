if (QT5)
    find_package(Qt5 REQUIRED COMPONENTS
            Gui
            Widgets)
else()
    find_package(Qt6 REQUIRED COMPONENTS
            Gui
            Widgets)
endif()

add_library(qzdl_gui STATIC
        ZDLAboutDialog.cpp
        ZDLAboutDialog.h
        ZDLConfigurationManager.cpp
        ZDLConfigurationManager.h
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
        ZDLListable.cpp
        ZDLListable.h
        ZDLListEntry.cpp
        ZDLListEntry.hpp
        ZDLListWidget.cpp
        ZDLListWidget.h
        ZDLMainWindow.cpp
        ZDLMainWindow.h
        ZDLMultiPane.cpp
        ZDLMultiPane.h
        ZDLNameInput.cpp
        ZDLNameInput.h
        ZDLNameListable.cpp
        ZDLNameListable.h
        ZDLQSplitter.cpp
        ZDLQSplitter.h
        ZDLSettingsPane.cpp
        ZDLSettingsPane.h
        ZDLSettingsTab.cpp
        ZDLSettingsTab.h
        ZDLSourcePortList.cpp
        ZDLSourcePortList.h
        ZDLWidget.cpp
        ZDLWidget.h)

target_link_libraries(qzdl_gui
    PRIVATE
        qzdl_base
    PUBLIC
        Qt::Gui
        Qt::Widgets)

if (WIN32)
    target_sources(qzdl_gui PRIVATE
            win32/SimpleWFA.h
            win32/SimpleWFA.cpp
            win32/ZDLFileAssociations.h
            win32/ZDLFileAssociations.cpp)
endif()
