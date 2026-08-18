file(GLOB_RECURSE SABRE_PILOT_RUNNER_CORE_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_runner_core STATIC ${SABRE_PILOT_RUNNER_CORE_SOURCES})

target_link_libraries(
    sabre_pilot_runner_core
    PUBLIC
        sabre
        sabre_pilot_impl
        sabre_ipc_tcp
)

set_target_properties(
    sabre_pilot_runner_core
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot_runner_core
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ${PROJECT_SOURCE_DIR}/src/runner/
)