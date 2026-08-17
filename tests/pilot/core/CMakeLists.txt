find_package(GTest REQUIRED CONFIG)

file(GLOB test_sabre_pilot_core_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_pilot_core ${test_sabre_pilot_core_SOURCES})

target_link_libraries(test_sabre_pilot_core
    PRIVATE
        sabre_pilot_core
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(test_sabre_pilot_core PRIVATE --coverage)
target_link_options(test_sabre_pilot_core PRIVATE --coverage)

target_include_directories(
    test_sabre_pilot_core
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/pilot/
)

gtest_discover_tests(test_sabre_pilot_core)
