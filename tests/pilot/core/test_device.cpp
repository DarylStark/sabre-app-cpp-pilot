#include "mock_subprocess_strategy.hpp"
#include <core/device.hpp>
#include <gtest/gtest.h>

TEST(DeviceTests, Construct)
{
    MockSubprocessStrategy mockStrategy;

    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre_pilot::DeviceConfig deviceConfig{config, "non_library.so",
                                           "startApp"};
    sabre_pilot::Device device(1, deviceConfig, mockStrategy, "");
    ASSERT_EQ(device.getUartCount(), 1);
}