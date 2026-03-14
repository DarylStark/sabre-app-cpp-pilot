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

file(GLOB_RECURSE IMGUI_PRESENTER_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(imgui_presenter STATIC ${IMGUI_PRESENTER_SOURCES})

target_link_libraries(imgui_presenter PRIVATE imgui sabre_pilot glfw OpenGL)

target_include_directories(
    imgui_presenter
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../simulator/
)