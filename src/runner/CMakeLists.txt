if ("${SABRE_PILOT_BUILD_RUNNER_APP}" STREQUAL "ON")
    add_subdirectory("app")
endif()

if ("${SABRE_PILOT_BUILD_RUNNER_CORE}" STREQUAL "ON")
    add_subdirectory("core")
endif()

if ("${SABRE_PILOT_BUILD_RUNNER_SABRE_IMPL}" STREQUAL "ON")
    add_subdirectory("sabre_impl")
endif()