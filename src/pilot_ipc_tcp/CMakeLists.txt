find_package(asio REQUIRED)

file(GLOB_RECURSE SABRE_TCP_IPC_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_ipc_tcp STATIC ${SABRE_TCP_IPC_SOURCES})

target_link_libraries(sabre_pilot_ipc_tcp PUBLIC asio::asio)

set_target_properties(sabre_pilot_ipc_tcp
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    sabre_pilot_ipc_tcp
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../
)