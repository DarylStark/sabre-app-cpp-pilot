#include <gtest/gtest.h>
#include <pilot/device.hpp>
#include <pilot/exceptions.hpp>

TEST(Device, CreateDeviceNoUart)
{
    // Should not throw an exception
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 0};
    sabre_pilot::Device device(config);
}

TEST(Device, RetrieveSameUartTwice)
{
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 1};
    sabre_pilot::Device device(config);
    auto &uart_1 = device.outputUart(0);
    auto &uart_2 = device.outputUart(0);
    ASSERT_EQ(&uart_1, &uart_2);
}

TEST(Device, ExceptionOnRetrievingNonExistingUart)
{
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 1};
    sabre_pilot::Device device(config);
    ASSERT_THROW(device.outputUart(1),
                 sabre_pilot::exceptions::DeviceUartNotConfiguredException);
}
