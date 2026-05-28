#include <gtest/gtest.h>
#include <memory>
#include <pilot/device.hpp>
#include <pilot/exceptions.hpp>
#include <pilot_impl/exceptions.hpp>
#include <pilot_impl/hal.hpp>

class HalUart : public ::testing::Test
{
public:
    HalUart()
    {
        sabre::core::ResourceManagerConfig config = {.maxGpios = 30,
                                                     .upperboundUart = 3};
        _device = std::make_unique<sabre_pilot::Device>(config);
    }

protected:
    std::unique_ptr<sabre_pilot::Device> _device;
};

TEST_F(HalUart, CreateWithoutDevice)
{
    ASSERT_THROW(sabre::impl::pilot::Uart uart(nullptr, 0),
                 sabre::impl::pilot::DeviceNotConfiguredException);
}

TEST_F(HalUart, CreateUnitialized)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    ASSERT_FALSE(uart.isInitialized());
}

TEST_F(HalUart, CreateInvalidYartDevice)
{
    ASSERT_THROW(sabre::impl::pilot::Uart uart(_device.get(), 5),
                 sabre::impl::pilot::InvalidUartIndex);
}

TEST_F(HalUart, Initialize)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    uart.initialize();
    ASSERT_TRUE(uart.isInitialized());
}

TEST_F(HalUart, Deinitialize)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    uart.initialize();
    ASSERT_TRUE(uart.isInitialized());
    uart.deinitialize();
    ASSERT_FALSE(uart.isInitialized());
}

TEST_F(HalUart, WriteAndFlushData)
{
    std::string output = "";
    _device->getUartController(0).setOutputBufferCallback(
        [&output](char c) { output.push_back(c); });
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    uart.initialize();
    uart.writeByte('d');
    uart.flush();
    ASSERT_EQ(output, "d");
}

TEST_F(HalUart, WriteWithoutFlushData)
{
    std::string output = "";
    _device->getUartController(0).setOutputBufferCallback(
        [&output](char c) { output.push_back(c); });
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    uart.initialize();
    uart.writeByte('d');
    ASSERT_EQ(output, "");
}

TEST_F(HalUart, WriteDataReturnValue)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    uart.initialize();
    ASSERT_EQ(uart.writeByte('d'), 1);
}

TEST_F(HalUart, WriteDataBeforeInitialized)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    ASSERT_THROW(
        uart.writeByte('d'),
        sabre_pilot::exceptions::UartControllerNotInitializedException);
}

TEST_F(HalUart, FlushDataBeforeInitialized)
{
    sabre::impl::pilot::Uart uart(_device.get(), 0);
    ASSERT_THROW(
        uart.flush(),
        sabre_pilot::exceptions::UartControllerNotInitializedException);
}

class HalGpio : public ::testing::Test
{
public:
    HalGpio()
    {
        sabre::core::ResourceManagerConfig config = {.maxGpios = 30,
                                                     .upperboundUart = 3};
        _device = std::make_unique<sabre_pilot::Device>(config);
    }

protected:
    std::unique_ptr<sabre_pilot::Device> _device;
};

TEST_F(HalGpio, CreateWithoutDevice)
{
    ASSERT_THROW(sabre::impl::pilot::Gpio gpio(nullptr, 1),
                 sabre::impl::pilot::DeviceNotConfiguredException);
}
