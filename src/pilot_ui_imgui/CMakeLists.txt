file(GLOB_RECURSE SABRE_PILOT_UI_IMGUI_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ui_imgui STATIC ${SABRE_PILOT_UI_IMGUI_SOURCES})

target_link_libraries(sabre_pilot_ui_imgui PUBLIC sabre_pilot_core)

target_include_directories(
    sabre_pilot_ui_imgui
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)