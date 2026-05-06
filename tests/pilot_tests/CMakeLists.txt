file(GLOB SABRE_PILOT_TESTS_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(sabre_pilot_tests ${SABRE_PILOT_TESTS_SOURCES})

set(BUILD_GMOCK OFF)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(sabre_pilot_tests PRIVATE sabre_pilot gtest gtest_main)

target_compile_options(sabre_pilot_tests PRIVATE --coverage)
target_link_options(sabre_pilot_tests PRIVATE --coverage)

target_include_directories(
    sabre_pilot_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/
)

include(GoogleTest)
gtest_discover_tests(sabre_pilot_tests)