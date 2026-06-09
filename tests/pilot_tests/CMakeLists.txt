file(GLOB sabre_pilot_tests_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(sabre_pilot_tests ${sabre_pilot_tests_SOURCES})

target_link_libraries(sabre_pilot_tests PRIVATE sabre_pilot_core gtest gtest_main)

target_compile_options(sabre_pilot_tests PRIVATE --coverage)
target_link_options(sabre_pilot_tests PRIVATE --coverage)

target_include_directories(
    sabre_pilot_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/
    ${sabre_SOURCE_DIR}/src/sabre/
)

gtest_discover_tests(sabre_pilot_tests)

add_subdirectory(test_dynamic_lib_1)
add_subdirectory(test_dynamic_lib_2)