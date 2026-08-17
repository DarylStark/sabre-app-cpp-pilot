find_package(GTest REQUIRED CONFIG)

file(GLOB TEST_SABRE_PILOT_IMPL_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_pilot_impl ${TEST_SABRE_PILOT_IMPL_SOURCES})

target_link_libraries(test_sabre_pilot_impl
    PRIVATE
        sabre_pilot_impl
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(test_sabre_pilot_impl PRIVATE --coverage)
target_link_options(test_sabre_pilot_impl PRIVATE --coverage)

target_include_directories(
    test_sabre_pilot_impl
    PRIVATE
        ${sabre_SOURCE_DIR}/src/sabre/
        ${PROJECT_SOURCE_DIR}/src/shared/
        ${PROJECT_SOURCE_DIR}/src/runner/
)

gtest_discover_tests(test_sabre_pilot_impl)