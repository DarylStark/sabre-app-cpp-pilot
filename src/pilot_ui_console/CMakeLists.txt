file(GLOB_RECURSE SABRE_PILOT_UI_CONSOLE_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ui_console SHARED ${SABRE_PILOT_UI_CONSOLE_SOURCES})

target_link_libraries(sabre_pilot_ui_console PUBLIC sabre_pilot_core)

set_target_properties(sabre_pilot_ui_console
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot_ui_console
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)