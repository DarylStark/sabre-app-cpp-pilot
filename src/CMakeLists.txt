include(FetchContent)
FetchContent_Declare(
    sabre
    GIT_REPOSITORY https://github.com/DarylStark/sabre-lib-cpp-sabre.git
    GIT_TAG dev
)
FetchContent_MakeAvailable(sabre)

add_subdirectory("pilot")
add_subdirectory("pilot_exec")
add_subdirectory("pilot_impl")