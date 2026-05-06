file(GLOB_RECURSE SABRE_PILOT_IMPL_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(sabre_pilot_impl STATIC ${SABRE_PILOT_IMPL_SOURCES})

target_link_libraries(sabre_pilot_impl PUBLIC sabre)

target_include_directories(
    sabre_pilot_impl
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
)