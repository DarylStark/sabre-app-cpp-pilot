file(GLOB_RECURSE SABRE_PILOT_CORE_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_core STATIC ${SABRE_PILOT_CORE_SOURCES})

set_target_properties(sabre_pilot_core
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(
    sabre_pilot_core
    PRIVATE
        wuphf
    PUBLIC
        sabre_ipc_tcp
)

target_include_directories(
    sabre_pilot_core
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)