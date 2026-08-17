# Pilot application
file(GLOB_RECURSE EXAMPLE_APP_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(example_app SHARED ${EXAMPLE_APP_SOURCES})

target_link_libraries(
    example_app
    PUBLIC
        sabre
)
set_target_properties(
    example_app
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON)

target_include_directories(
    example_app
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
)