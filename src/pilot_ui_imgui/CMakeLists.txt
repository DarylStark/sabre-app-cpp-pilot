# GLFW (installed via Conan)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(IMGUI_SOURCE_FOLDER "${imgui_PACKAGE_FOLDER_RELEASE}")
else()
    set(IMGUI_SOURCE_FOLDER "${imgui_PACKAGE_FOLDER_DEBUG}")
endif()

file(GLOB_RECURSE SABRE_PILOT_UI_IMGUI_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(
    sabre_pilot_ui_imgui STATIC
        ${SABRE_PILOT_UI_IMGUI_SOURCES}
        "${IMGUI_SOURCE_FOLDER}/res/bindings/imgui_impl_glfw.cpp"
        "${IMGUI_SOURCE_FOLDER}/res/bindings/imgui_impl_opengl3.cpp"
)

set_target_properties(sabre_pilot_ui_imgui PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sabre_pilot_ui_imgui PUBLIC sabre_pilot_core imgui::imgui glfw OpenGL)

target_include_directories(
    sabre_pilot_ui_imgui
    PUBLIC
        ${IMGUI_SOURCE_FOLDER}/res/bindings
        ${sabre_SOURCE_DIR}/src/sabre/
)
