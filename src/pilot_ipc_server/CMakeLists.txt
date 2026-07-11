file(GLOB_RECURSE SABRE_PILOT_IPC_SERVER_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ipc_server STATIC ${SABRE_PILOT_IPC_SERVER_SOURCES})

set_target_properties(sabre_pilot_ipc_server
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot_ipc_server
    PUBLIC
        ../
)