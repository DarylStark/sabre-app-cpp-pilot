find_package(asio REQUIRED)

add_library(ipc_tcp INTERFACE)

target_link_libraries(
    ipc_tcp
    INTERFACE
        asio::asio
)

target_include_directories(ipc_tcp INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
    ../
)