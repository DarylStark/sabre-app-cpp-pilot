find_package(GTest REQUIRED CONFIG)

file(GLOB TEST_SABRE_PILOT_CORE_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_pilot_core ${TEST_SABRE_PILOT_CORE_SOURCES})

target_link_libraries(test_sabre_pilot_core
    PRIVATE
        sabre_pilot_core
        sabre_wuphf
        GTest::gtest
        GTest::gtest_main
        sabre_ipc_tcp
        sabre_ipc
)

target_compile_options(test_sabre_pilot_core PRIVATE --coverage)
target_link_options(test_sabre_pilot_core PRIVATE --coverage)

target_include_directories(
    test_sabre_pilot_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/pilot/
        ${PROJECT_SOURCE_DIR}/src/shared/
)

gtest_discover_tests(test_sabre_pilot_core)
