find_package(GTest REQUIRED CONFIG)

file(GLOB test_sabre_pilot_runner_core_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_pilot_runner_core ${test_sabre_pilot_runner_core_SOURCES})

target_link_libraries(test_sabre_pilot_runner_core
    PRIVATE
        sabre_pilot_runner_core
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(test_sabre_pilot_runner_core PRIVATE --coverage)
target_link_options(test_sabre_pilot_runner_core PRIVATE --coverage)

target_include_directories(
    test_sabre_pilot_runner_core
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/
        ${sabre_SOURCE_DIR}/src/sabre/
)

gtest_discover_tests(test_sabre_pilot_runner_core)

add_subdirectory(test_dynamic_lib_1)
add_subdirectory(test_dynamic_lib_2)