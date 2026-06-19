# Pilot application
file(GLOB_RECURSE SABRE_PILOT_EXEC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(sabre_pilot ${SABRE_PILOT_EXEC_SOURCES})

# TOML++
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

target_link_libraries(sabre_pilot PUBLIC sabre_pilot_core sabre_pilot_ui_imgui sabre_pilot_ui_console tomlplusplus::tomlplusplus)

target_include_directories(
    sabre_pilot
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)