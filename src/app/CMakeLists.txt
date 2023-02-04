add_executable(qzdl_app)

target_sources(qzdl_app PRIVATE qzdl.cpp)

target_link_libraries(qzdl_app PRIVATE
        qzdl_base
        qzdl_gui)

set_target_properties(qzdl_app PROPERTIES OUTPUT_NAME qzdl)