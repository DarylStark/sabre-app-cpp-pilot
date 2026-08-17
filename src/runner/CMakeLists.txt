if(SABRE_PILOT_BUILD_RUNNER_APP)
    add_subdirectory(app)
endif()

if(SABRE_PILOT_BUILD_RUNNER_CORE)
    add_subdirectory(core)
endif()

if(SABRE_PILOT_BUILD_RUNNER_SABRE_IMPL)
    add_subdirectory(sabre_impl)
endif()