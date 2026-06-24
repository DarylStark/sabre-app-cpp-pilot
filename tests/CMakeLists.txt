include(GoogleTest)

if(SABRE_PILOT_BUILD_TESTS STREQUAL "ON")
    add_subdirectory(test_sabre_pilot_core)
    add_subdirectory(test_sabre_pilot_impl)
    add_subdirectory(test_sabre_pilot_runner_core)
endif()