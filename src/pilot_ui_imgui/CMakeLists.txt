# ImGui
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

file(
    GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
set_target_properties(imgui PROPERTIES CXX_CLANG_TIDY "")

target_include_directories(
    imgui
    PRIVATE
        ${imgui_SOURCE_DIR}
)

file(GLOB_RECURSE SABRE_PILOT_UI_IMGUI_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(
    sabre_pilot_ui_imgui STATIC
        ${SABRE_PILOT_UI_IMGUI_SOURCES}
)

target_link_libraries(sabre_pilot_ui_imgui PUBLIC sabre_pilot_core imgui glfw OpenGL)
set_target_properties(sabre_pilot_ui_imgui PROPERTIES CXX_CLANG_TIDY "")

target_include_directories(
    sabre_pilot_ui_imgui
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ${imgui_SOURCE_DIR}
)