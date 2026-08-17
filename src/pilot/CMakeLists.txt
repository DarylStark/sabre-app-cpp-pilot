if ("${SABRE_PILOT_BUILD_PILOT_APP}" STREQUAL "ON")
    add_subdirectory("app")
endif()

if ("${SABRE_PILOT_BUILD_PILOT_CORE}" STREQUAL "ON")
    add_subdirectory("core")
endif()

if ("${SABRE_PILOT_BUILD_PILOT_UI_CONSOLE}" STREQUAL "ON")
    add_subdirectory("ui_console")
endif()

if ("${SABRE_PILOT_BUILD_PILOT_UI_IMGUI}" STREQUAL "ON")
    add_subdirectory("ui_imgui")
endif()