#include <gtest/gtest.h>
#include <pilot_core/pilot.hpp>

TEST(PilotTests, Construct)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();
    ASSERT_EQ(devices.size(), 0);
}

TEST(PilotTests, AddOneDevice)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device", config,
                    "test_dynamic_lib_1/libtest_dynamic_lib_1.so",
                    "startApp_0");

    ASSERT_EQ(devices.size(), 1);
}

TEST(PilotTests, AddTwoDevices)
{
    sabre_pilot::Pilot pilot;
    const auto &devices = pilot.getDeviceMap();

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    pilot.addDevice("test_device_1", config,
                    "test_dynamic_lib_1/libtest_dynamic_lib_1.so",
                    "startApp_0");
    pilot.addDevice("test_device_2", config,
                    "test_dynamic_lib_2/libtest_dynamic_lib_2.so",
                    "startApp_0");

    ASSERT_EQ(devices.size(), 2);
}
