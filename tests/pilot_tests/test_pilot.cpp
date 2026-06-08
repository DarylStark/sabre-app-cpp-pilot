#include <gtest/gtest.h>
#include <pilot_core/pilot.hpp>

TEST(PilotTests, Construct)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    const auto &libraries = pilot.getLibraryMap();
    ASSERT_EQ(devices.size(), 0);
    ASSERT_EQ(libraries.size(), 0);
}

TEST(PilotTests, AddOneDevice)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    const auto &libraries = pilot.getLibraryMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device", config, "testlib.so", "startApp");

    ASSERT_EQ(devices.size(), 1);
    ASSERT_EQ(libraries.size(), 1);
}

TEST(PilotTests, AddTwoDevices)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    const auto &libraries = pilot.getLibraryMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device_1", config, "testlib_1.so", "startApp");
    pilot.addDevice("test_device_2", config, "testlib_2.so", "startApp");

    ASSERT_EQ(devices.size(), 2);
    ASSERT_EQ(libraries.size(), 2);
}

TEST(PilotTests, AddTwoDevicesWithSameLibraryAndEntryPoint)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    const auto &libraries = pilot.getLibraryMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device_1", config, "testlib.so", "startApp");
    pilot.addDevice("test_device_2", config, "testlib.so", "startApp");

    ASSERT_EQ(devices.size(), 2);
    ASSERT_EQ(libraries.size(), 1);
}

TEST(PilotTests, AddTwoDevicesWithSameLibraryDifferentEntryPoint)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    const auto &libraries = pilot.getLibraryMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device_1", config, "testlib.so", "startApp_1");
    pilot.addDevice("test_device_2", config, "testlib.so", "startApp_2");

    ASSERT_EQ(devices.size(), 2);
    ASSERT_EQ(libraries.size(), 1);
}