add_library(qzdl_base STATIC
        external/miniz/miniz.h
        external/miniz/miniz.c
        #config.h
        libwad.cpp
        libwad.h
        zdlcommon.h
        zdlconf.cpp
        zdlconf.hpp
        ZDLConfiguration.cpp
        ZDLConfiguration.h
        ZDLFileInfo.cpp
        ZDLFileInfo.h
        zdlline.cpp
        zdlline.hpp
        ZDLMapFile.cpp
        ZDLMapFile.h
        ZDLNullDevice.h
        zdlsection.cpp
        zdlsection.hpp
        ZLibDir.cpp
        ZLibDir.h
        ZLibPK3.cpp
        ZLibPK3.h)

target_include_directories(qzdl_base
    PRIVATE
        external/miniz
        external/rapidjson
    PUBLIC
        xpm)

target_link_libraries(qzdl_base PUBLIC
        Qt::Core)