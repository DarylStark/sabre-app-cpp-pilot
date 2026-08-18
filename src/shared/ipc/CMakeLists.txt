file(GLOB_RECURSE SABRE_IPC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_ipc SHARED ${SABRE_IPC_SOURCES})

set_target_properties(
    sabre_ipc
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_ipc
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/shared/
)