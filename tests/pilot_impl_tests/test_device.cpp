#include <gtest/gtest.h>
#include <pilot_impl/exceptions.hpp>
#include <pilot_impl/mcu.hpp>

TEST(Device, CreateDeviceNoUart)
{
    // Should not throw an exception
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 0};
    sabre::impl::pilot::Mcu device(config);
}

TEST(Device, RetrieveSameUartTwice)
{
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 1};
    sabre::impl::pilot::Mcu device(config);
    auto &uart_1 = device.getUartController(0);
    auto &uart_2 = device.getUartController(0);
    ASSERT_EQ(&uart_1, &uart_2);
}

TEST(Device, ExceptionOnRetrievingNonExistingUart)
{
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 1};
    sabre::impl::pilot::Mcu device(config);
    ASSERT_THROW(device.getUartController(1),
                 sabre::impl::pilot::DeviceUartNotConfiguredException);
}

TEST(Device, RetrievingConfig)
{
    sabre::core::ResourceManagerConfig config = {.upperboundUart = 990};
    sabre::impl::pilot::Mcu device(config);
    const sabre::core::ResourceManagerConfig &retrievedConfig =
        device.getConfig();
    ASSERT_EQ(retrievedConfig.upperboundUart, config.upperboundUart);
}
