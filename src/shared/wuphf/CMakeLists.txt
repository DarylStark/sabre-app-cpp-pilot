file(GLOB_RECURSE SABRE_WUPHF_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_wuphf STATIC ${SABRE_WUPHF_SOURCES})

target_link_libraries(
    sabre_wuphf
    PRIVATE
        sabre_pilot_core
        sabre_ipc
)

set_target_properties(
    sabre_wuphf
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_wuphf
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/shared/
        ${PROJECT_SOURCE_DIR}/src/pilot/
)