find_package(asio REQUIRED)

file(GLOB_RECURSE SABRE_IPC_TCP_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_ipc_tcp SHARED ${SABRE_IPC_TCP_SOURCES})

set_target_properties(
    sabre_ipc_tcp
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(
    sabre_ipc_tcp
    PUBLIC
        asio::asio
    PRIVATE
        sabre_ipc
)

target_include_directories(
    sabre_ipc_tcp
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/shared/
)