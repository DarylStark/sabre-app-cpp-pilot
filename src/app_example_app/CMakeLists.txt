# Pilot application
file(GLOB_RECURSE APP_EXAMPLE_APP CONFIGURE_DEPENDS *.cpp)
add_library(app_example_app SHARED ${APP_EXAMPLE_APP})

target_link_libraries(app_example_app PUBLIC sabre)
set_target_properties(app_example_app PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(
    app_example_app
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)