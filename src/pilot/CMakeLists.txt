find_package(tomlplusplus REQUIRED)
find_package(CLI11 REQUIRED)

file(GLOB_RECURSE SABRE_PILOT_EXEC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(sabre_pilot ${SABRE_PILOT_EXEC_SOURCES})

target_link_libraries(
    sabre_pilot
    PUBLIC
        sabre_pilot_core
        sabre_pilot_ui_console
        tomlplusplus::tomlplusplus
        CLI11::CLI11
)

set_target_properties(sabre_pilot
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)