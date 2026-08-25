#include <gtest/gtest.h>
#include <memory>
#include <sabre/core/resource_manager.hpp>
#include <sabre_impl/exceptions.hpp>
#include <sabre_impl/hal.hpp>

class HalUart : public ::testing::Test
{
public:
    HalUart()
    {
        sabre::core::ResourceManagerConfig config = {.maxGpios = 30,
                                                     .upperboundUart = 3};
    }
};

TEST_F(HalUart, CreateWithoutDevice)
{
    ASSERT_THROW(sabre::impl::pilot::Uart uart(0),
                 sabre::impl::pilot::DeviceNotConfiguredException);
}

TEST_F(HalUart, CreateUnitialized)
{
    sabre::impl::pilot::Uart uart(0);
    ASSERT_FALSE(uart.isInitialized());
}

TEST_F(HalUart, CreateInvalidYartDevice)
{
    ASSERT_THROW(sabre::impl::pilot::Uart uart(5),
                 sabre::impl::pilot::InvalidUartIndex);
}

TEST_F(HalUart, Initialize)
{
    sabre::impl::pilot::Uart uart(0);
    uart.initialize();
    ASSERT_TRUE(uart.isInitialized());
}

TEST_F(HalUart, Deinitialize)
{
    sabre::impl::pilot::Uart uart(0);
    uart.initialize();
    ASSERT_TRUE(uart.isInitialized());
    uart.deinitialize();
    ASSERT_FALSE(uart.isInitialized());
}

TEST_F(HalUart, WriteAndFlushData)
{
    ASSERT_TRUE(false);
}

TEST_F(HalUart, WriteWithoutFlushData)
{
    ASSERT_TRUE(false);
}

TEST_F(HalUart, WriteDataReturnValue)
{
    sabre::impl::pilot::Uart uart(0);
    uart.initialize();
    ASSERT_EQ(uart.writeByte('d'), 1);
}

TEST_F(HalUart, WriteDataBeforeInitialized)
{
    sabre::impl::pilot::Uart uart(0);
    ASSERT_THROW(uart.writeByte('d'),
                 sabre::impl::pilot::UartControllerNotInitializedException);
}

TEST_F(HalUart, FlushDataBeforeInitialized)
{
    sabre::impl::pilot::Uart uart(0);
    ASSERT_THROW(uart.flush(),
                 sabre::impl::pilot::UartControllerNotInitializedException);
}

class HalGpio : public ::testing::Test
{
public:
    HalGpio()
    {
        sabre::core::ResourceManagerConfig config = {.maxGpios = 30,
                                                     .upperboundUart = 3};
    }
};

TEST_F(HalGpio, CreateWithoutDevice)
{
    ASSERT_TRUE(false);
}
