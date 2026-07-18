cmake_minimum_required(VERSION 3.20)
project(sabre LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

include(FetchContent)
FetchContent_Declare(
    sabre
    GIT_REPOSITORY https://github.com/DarylStark/sabre-lib-cpp-sabre.git
    GIT_TAG dev
)
FetchContent_MakeAvailable(sabre)

if(SABRE_PILOT_BUILD_TESTS STREQUAL "ON")
    message(STATUS "Enabling testing")
    enable_testing()
    message(STATUS "Enabling coverage flags")
    add_compile_options(--coverage)
    add_link_options(--coverage)
    add_subdirectory(tests)
endif()

# Application source
add_subdirectory(src/)

