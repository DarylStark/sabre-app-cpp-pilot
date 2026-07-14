add_subdirectory("pilot_impl")
add_subdirectory("pilot_core")
add_subdirectory("pilot_runner_core")

if(SABRE_PILOT_BUILD_PILOT_EXEC)
    add_subdirectory("pilot")
ENDIF()

if(SABRE_PILOT_BUILD_PILOT_RUNNER)
    add_subdirectory("pilot_runner")
ENDIF()

if(SABRE_PILOT_BUILD_EXAMPLE_APPS)
    add_subdirectory("app_example_app")
endif()

if(SABRE_PILOT_BUILD_UI_LIBRARIES)
    add_subdirectory("pilot_ui_imgui")
    add_subdirectory("pilot_ui_console")
endif()

if(SABRE_PILOT_BUILD_IPC_LIBRARIES)
    add_subdirectory("pilot_ipc_server")        # TODO: Remoe
    add_subdirectory("pilot_ipc_tcp")           # TODO: Remoe
    add_subdirectory("pilot_ipc_protocol")      # TODO: Remoe
    add_subdirectory("pilot_ipc_protocol_v1")   # TODO: Remoe
    add_subdirectory("wuphf")
endif()