find_package(GTest REQUIRED CONFIG)

file(GLOB TEST_SABRE_IPC_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_ipc ${TEST_SABRE_IPC_SOURCES})

target_link_libraries(test_sabre_ipc
    PRIVATE
        sabre_ipc
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(test_sabre_ipc PRIVATE --coverage)
target_link_options(test_sabre_ipc PRIVATE --coverage)

target_include_directories(
    test_sabre_ipc
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/shared/
)

gtest_discover_tests(test_sabre_ipc)