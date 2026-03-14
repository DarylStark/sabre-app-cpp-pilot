include(FetchContent)
FetchContent_Declare(
    sabre
    GIT_REPOSITORY https://github.com/DarylStark/sabre-lib-cpp-sabre.git
    GIT_TAG dev
)
FetchContent_MakeAvailable(sabre)

# Pilot Library
file(GLOB_RECURSE SABRE_PILOT_SOURCES CONFIGURE_DEPENDS sabre_pilot/*.cpp simulator/*.cpp)
add_library(sabre_pilot STATIC ${SABRE_PILOT_SOURCES})

target_link_libraries(sabre_pilot PUBLIC sabre glfw OpenGL)

target_include_directories(
    sabre_pilot
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${sabre_SOURCE_DIR}/src/sabre/
)

add_subdirectory("imgui_presenter/")

# Temporary executable
# TODO: Remove
add_executable(sabre_pilot_exec main.cpp)
target_link_libraries(
        sabre_pilot_exec
    PRIVATE
        sabre_pilot
        imgui_presenter
)

target_include_directories(
    sabre_pilot_exec
    PRIVATE
        ${sabre_SOURCE_DIR}/src/sabre/
)
