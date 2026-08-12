file(GLOB_RECURSE WUPHF_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(wuphf STATIC ${WUPHF_SOURCES})

target_link_libraries(
    wuphf
    PRIVATE
        sabre_pilot_core
)

set_target_properties(wuphf
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    wuphf
    PUBLIC
        ../
)