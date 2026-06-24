# Pilot application
file(GLOB_RECURSE TEST_DYNAMIC_LIB_2_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(test_dynamic_lib_2 SHARED ${TEST_DYNAMIC_LIB_2_SOURCES})

target_link_libraries(test_dynamic_lib_2 PUBLIC sabre)
set_target_properties(test_dynamic_lib_2 PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(
    test_dynamic_lib_2
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../../src/
)