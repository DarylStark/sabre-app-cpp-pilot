if(SABRE_PILOT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SABRE_PILOT_BUILD_PILOT)
    add_subdirectory(pilot)
endif()

if(SABRE_PILOT_BUILD_RUNNER)
    add_subdirectory(runner)
endif()

add_subdirectory(shared)