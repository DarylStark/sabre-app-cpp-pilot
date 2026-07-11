file(GLOB_RECURSE SABRE_PILOT_IPC_PROTOCOL_V1_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ipc_protocol_v1 STATIC ${SABRE_PILOT_IPC_PROTOCOL_V1_SOURCES})

target_include_directories(
    sabre_pilot_ipc_protocol_v1
    PUBLIC
        ../
)

target_link_libraries(
    sabre_pilot_ipc_protocol_v1
    PUBLIC
        sabre_pilot_ipc_protocol
)

set_target_properties(sabre_pilot_ipc_protocol_v1
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)