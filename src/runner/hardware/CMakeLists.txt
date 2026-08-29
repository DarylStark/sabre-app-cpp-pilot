file(GLOB_RECURSE SABRE_PILOT_IMPL_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_hardware STATIC ${SABRE_PILOT_IMPL_SOURCES})

target_link_libraries(
    sabre_pilot_hardware
    PUBLIC
        sabre_ipc
        sabre_wuphf
)

set_target_properties(
    sabre_pilot_hardware
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot_hardware
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/runner/
)