# Pilot application
file(GLOB_RECURSE APP_EXAMPLE_APP CONFIGURE_DEPENDS *.cpp)
add_library(example_app SHARED ${APP_EXAMPLE_APP})

target_link_libraries(example_app PUBLIC sabre)
set_target_properties(example_app PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(
    example_app
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
)