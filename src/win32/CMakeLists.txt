add_library(zdl_win32 STATIC
        SimpleWFA.h
        SimpleWFA.cpp
        ZDLFileAssociations.h
        ZDLFileAssociations.cpp)

target_link_libraries(zdl_win32
        PRIVATE
        qbt_base
        PUBLIC
        Qt::Gui Qt::Widgets
        )
