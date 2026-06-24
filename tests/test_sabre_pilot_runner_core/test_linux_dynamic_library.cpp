#include <gtest/gtest.h>
#include <pilot_runner_core/exceptions.hpp>
#include <pilot_runner_core/linux_dynamic_library.hpp>
#include <sabre/core/resource_manager.hpp>

TEST(LinuxDynamicLibraryTest, GetExisitingEntryPoint)
{
    sabre_pilot_runner_core::LinuxDynamicLibrary lib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    const auto ep0 = lib.getEntryPoint("startApp_0");
    ASSERT_NE(ep0, nullptr);
}

TEST(LinuxDynamicLibraryTest, GetTwoEntryPoints)
{
    sabre_pilot_runner_core::LinuxDynamicLibrary lib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    const auto ep0 = lib.getEntryPoint("startApp_0");
    const auto ep1 = lib.getEntryPoint("startApp_1");
    ASSERT_NE(ep0, nullptr);
    ASSERT_NE(ep1, nullptr);
}

TEST(LinuxDynamicLibraryTest, RetrieveNonExistingEntryPoint)
{
    sabre_pilot_runner_core::LinuxDynamicLibrary lib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    ASSERT_THROW(lib.getEntryPoint("nonExistingEntryPoint"),
                 sabre_pilot_runner_core::EntryPointNotInLibraryException);
}

TEST(LinuxDynamicLibraryTest, LoadNonExistingLibrary)
{
    sabre_pilot_runner_core::LinuxDynamicLibrary lib("libnon_existing_lib.so");
    ASSERT_THROW(lib.getEntryPoint("nonExistingEntryPoint"),
                 sabre_pilot_runner_core::DynamicLibraryLoadingException);
}