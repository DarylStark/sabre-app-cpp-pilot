file(GLOB_RECURSE SABRE_PILOT_IPC_PROTOCOL_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ipc_protocol STATIC ${SABRE_PILOT_IPC_PROTOCOL_SOURCES})

target_link_libraries(
    sabre_pilot_ipc_protocol
    PUBLIC
        sabre_pilot_ipc_server
)

target_include_directories(
    sabre_pilot_ipc_protocol
    PUBLIC
        ../
)

set_target_properties(sabre_pilot_ipc_protocol
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)