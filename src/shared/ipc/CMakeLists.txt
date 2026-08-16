file(GLOB_RECURSE SABRE_IPC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_ipc STATIC ${SABRE_IPC_SOURCES})

set_target_properties(
    sabre_ipc
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

