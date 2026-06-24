include(GoogleTest)

if(SABRE_PILOT_BUILD_TESTS STREQUAL "ON")
    add_subdirectory(pilot_impl_tests)
    add_subdirectory(pilot_tests)
endif()