add_subdirectory("pilot_impl")
add_subdirectory("pilot_core")

if(SABRE_PILOT_BUILD_PILOT_EXEC)
    add_subdirectory("pilot")
ENDIF()

if(SABRE_PILOT_BUILD_EXAMPLE_APPS)
    add_subdirectory("app_example_app")
endif()

if(SABRE_PILOT_BUILD_UI_LIBRARIES)
    add_subdirectory("pilot_ui_imgui")
    add_subdirectory("pilot_ui_console")
endif()