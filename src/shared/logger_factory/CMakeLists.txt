find_package(spdlog REQUIRED)

file(GLOB_RECURSE LOGGER_FACTORY_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_logger_factory SHARED ${LOGGER_FACTORY_SOURCES})

set_target_properties(
    sabre_logger_factory
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(
    sabre_logger_factory
    PUBLIC
        spdlog::spdlog
)

target_include_directories(
    sabre_logger_factory
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/shared/
)