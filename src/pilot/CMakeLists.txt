# Pilot application
file(GLOB_RECURSE SABRE_PILOT_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot STATIC ${SABRE_PILOT_SOURCES})

