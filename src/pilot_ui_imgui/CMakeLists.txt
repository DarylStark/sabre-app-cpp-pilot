# GLFW (installed via Conan)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)

# ImGui paths, needed for includes and compilation
get_target_property(IMGUI_INCLUDE_DIR imgui::imgui INTERFACE_INCLUDE_DIRECTORIES)
get_filename_component(IMGUI_ROOT_DIR "${IMGUI_INCLUDE_DIR}" DIRECTORY)
get_filename_component(IMGUI_BINDINGS_DIR "${IMGUI_ROOT_DIR}/res/bindings" ABSOLUTE)

file(GLOB_RECURSE SABRE_PILOT_UI_IMGUI_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(
    sabre_pilot_ui_imgui STATIC
        ${SABRE_PILOT_UI_IMGUI_SOURCES}
        "${imgui_PACKAGE_FOLDER_DEBUG}/res/bindings/imgui_impl_glfw.cpp"
        "${imgui_PACKAGE_FOLDER_DEBUG}/res/bindings/imgui_impl_opengl3.cpp"
)

set_target_properties(sabre_pilot_ui_imgui PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sabre_pilot_ui_imgui PUBLIC sabre_pilot_core imgui::imgui glfw OpenGL)

target_include_directories(
    sabre_pilot_ui_imgui
    PUBLIC
        ${imgui_PACKAGE_FOLDER_DEBUG}/res/bindings
        ${sabre_SOURCE_DIR}/src/sabre/
)
