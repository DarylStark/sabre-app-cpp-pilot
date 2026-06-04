#include <gtest/gtest.h>
#include <pilot_core/exceptions.hpp>
#include <pilot_core/linux_dynamic_library.hpp>
#include <pilot_impl/core.hpp>
#include <sabre/core/resource_manager.hpp>

TEST(LinuxDynamicLibraryTests, ErrorOnLoadingInvalidLibrary)
{
    sabre_pilot::LinuxDynamicLibrary dynlib("non_existing_lib.so");
    ASSERT_THROW(dynlib.load(), sabre_pilot::DynamicLibraryLoadingException);
}

TEST(LinuxDynamicLibraryTests, LoadLibrary)
{
    sabre_pilot::LinuxDynamicLibrary dynlib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    dynlib.load();
}

TEST(LinuxDynamicLibraryTests, LoadEntryPoints)
{
    sabre_pilot::LinuxDynamicLibrary dynlib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    dynlib.addEntryPoint("startApp_0");
    dynlib.load();
    auto fn = dynlib.getEntryPoint("startApp_0");
    ASSERT_NE(fn, nullptr);
}

TEST(LinuxDynamicLibraryTests, LoadNewEntryPointAfterLoading)
{
    sabre_pilot::LinuxDynamicLibrary dynlib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    dynlib.addEntryPoint("startApp_0");
    dynlib.load();

    dynlib.addEntryPoint("startApp_42");
    dynlib.load();

    auto fn = dynlib.getEntryPoint("startApp_42");
    ASSERT_NE(fn, nullptr);
}

TEST(LinuxDynamicLibraryTests, LoadEntryPointsCheckValue)
{
    sabre_pilot::LinuxDynamicLibrary dynlib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");
    sabre::impl::pilot::Factory fac(nullptr);
    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre::core::ResourceManager rm(fac, config);

    dynlib.addEntryPoint("startApp_0");
    dynlib.addEntryPoint("startApp_1");
    dynlib.addEntryPoint("startApp_42");
    dynlib.load();

    auto fn_0 = dynlib.getEntryPoint("startApp_0");
    auto fn_1 = dynlib.getEntryPoint("startApp_1");
    auto fn_42 = dynlib.getEntryPoint("startApp_42");

    ASSERT_EQ(fn_0(rm), 0);
    ASSERT_EQ(fn_1(rm), 1);
    ASSERT_EQ(fn_42(rm), 42);
}

TEST(LinuxDynamicLibraryTests, LoadInvalidEntryPoint)
{
    sabre_pilot::LinuxDynamicLibrary dynlib(
        "test_dynamic_lib_1/libtest_dynamic_lib_1.so");

    dynlib.addEntryPoint("startApp_55");
    ASSERT_THROW(dynlib.load(), sabre_pilot::EntryPointNotInLibraryException);
}