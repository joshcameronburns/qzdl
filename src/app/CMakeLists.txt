add_executable(qzdl_app)

if (WIN32)
    target_sources(qzdl_app PRIVATE
            qzdl.cpp
            resources/WIN32/qzdl.rc)
else()
    target_sources(qzdl_app PRIVATE
            qzdl.cpp)
endif()

target_link_libraries(qzdl_app PRIVATE
        qzdl_base
        qzdl_gui)

set_target_properties(qzdl_app PROPERTIES OUTPUT_NAME qzdl)