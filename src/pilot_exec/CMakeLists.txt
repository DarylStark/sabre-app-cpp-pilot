# Pilot application
file(GLOB_RECURSE SABRE_PILOT_EXEC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_executable(sabre_pilot_exec ${SABRE_PILOT_EXEC_SOURCES})

