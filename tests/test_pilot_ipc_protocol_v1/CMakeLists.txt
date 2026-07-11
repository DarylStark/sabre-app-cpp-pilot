find_package(GTest REQUIRED CONFIG)

file(GLOB test_sabre_pilot_ipc_protocol_v1_SOURCES CONFIGURE_DEPENDS test_*.cpp)
add_executable(test_sabre_pilot_ipc_protocol_v1 ${test_sabre_pilot_ipc_protocol_v1_SOURCES})

target_link_libraries(test_sabre_pilot_ipc_protocol_v1
    PRIVATE
        sabre_pilot_ipc_protocol
        sabre_pilot_ipc_protocol_v1
        GTest::gtest
        GTest::gtest_main
)

target_compile_options(test_sabre_pilot_ipc_protocol_v1 PRIVATE --coverage)
target_link_options(test_sabre_pilot_ipc_protocol_v1 PRIVATE --coverage)

target_include_directories(
    test_sabre_pilot_ipc_protocol_v1
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/
)

gtest_discover_tests(test_sabre_pilot_ipc_protocol_v1)
