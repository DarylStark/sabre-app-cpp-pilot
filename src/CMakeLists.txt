if(SABRE_PILOT_BUILD_EXAMPLES STREQUAL "ON")
    add_subdirectory("examples")
endif()

if(SABRE_PILOT_BUILD_PILOT STREQUAL "ON")
    add_subdirectory("pilot")
endif()

if(SABRE_PILOT_BUILD_RUNNER STREQUAL "ON")
    add_subdirectory("runner")
endif()

add_subdirectory("shared")