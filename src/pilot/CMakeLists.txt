if(SABRE_PILOT_BUILD_PILOT_APP)
    add_subdirectory(app)
endif()

if(SABRE_PILOT_BUILD_PILOT_CORE)
    add_subdirectory(core)
endif()

if(SABRE_PILOT_BUILD_PILOT_UI_CONSOLE)
    add_subdirectory(ui_console)
endif()

if(SABRE_PILOT_BUILD_PILOT_UI_IMGUI)
    add_subdirectory(ui_imgui)
endif()