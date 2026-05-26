file(GLOB SABRE_PILOT_IMPL_TESTS_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(sabre_pilot_impl_tests ${SABRE_PILOT_IMPL_TESTS_SOURCES})

target_link_libraries(sabre_pilot_impl_tests PRIVATE sabre_pilot sabre_pilot_impl gtest gtest_main)

target_compile_options(sabre_pilot_impl_tests PRIVATE --coverage)
target_link_options(sabre_pilot_impl_tests PRIVATE --coverage)

target_include_directories(
    sabre_pilot_impl_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/
    ${sabre_SOURCE_DIR}/src/sabre/
)

gtest_discover_tests(sabre_pilot_impl_tests)