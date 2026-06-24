# Pilot application
file(GLOB_RECURSE TEST_DYNAMIC_LIB_1_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(test_dynamic_lib_1 SHARED ${TEST_DYNAMIC_LIB_1_SOURCES})

target_link_libraries(test_dynamic_lib_1 PUBLIC sabre)
set_target_properties(test_dynamic_lib_1 PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(
    test_dynamic_lib_1
    PUBLIC
        ${sabre_SOURCE_DIR}/src/sabre/
        ../../src/
)