find_package(CLI11 REQUIRED)

file(GLOB_RECURSE SABRE_PILOT_RUNNER_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(sabre_pilot_runner ${SABRE_PILOT_RUNNER_SOURCES})

target_link_libraries(
    sabre_pilot_runner
    PRIVATE
        sabre_pilot_runner_core
    PUBLIC
        ipc_tcp
        wuphf
        CLI11::CLI11
)

target_include_directories(
    sabre_pilot_runner
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)

set_target_properties(sabre_pilot_runner
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)