# Pilot application
file(GLOB_RECURSE SABRE_PILOT_EXEC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(sabre_pilot ${SABRE_PILOT_EXEC_SOURCES})

target_link_libraries(sabre_pilot PUBLIC sabre_pilot_impl)

target_include_directories(
    sabre_pilot
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)