set(BUILD_GMOCK OFF)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

if(SABRE_PILOT_BUILD_TESTS STREQUAL "ON")
    add_subdirectory(pilot_impl_tests)
    add_subdirectory(pilot_tests)
endif()