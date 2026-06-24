#include <gtest/gtest.h>
#include <pilot_core/device.hpp>

TEST(DeviceTests, Construct)
{
    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre_pilot::DeviceConfig deviceConfig{config, "non_library.so",
                                           "startApp"};
    sabre_pilot::Device device(deviceConfig);
    ASSERT_EQ(device.getUartCount(), 1);
}