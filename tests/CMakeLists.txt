include(GoogleTest)

if(SABRE_PILOT_BUILD_PILOT STREQUAL "ON")
    add_subdirectory("pilot")
endif()

if(SABRE_PILOT_BUILD_RUNNER STREQUAL "ON")
    add_subdirectory("runner")
endif()
