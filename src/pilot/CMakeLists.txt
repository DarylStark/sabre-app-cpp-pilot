file(GLOB_RECURSE SABRE_PILOT_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot STATIC ${SABRE_PILOT_SOURCES})

target_link_libraries(sabre_pilot PUBLIC sabre)

target_include_directories(
    sabre_pilot
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
)
