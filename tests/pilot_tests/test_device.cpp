#include <gtest/gtest.h>
#include <pilot_core/device.hpp>

TEST(DeviceTests, Construct)
{
    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre_pilot::Device device(config);
    ASSERT_EQ(device.getUartCount(), 1);
}

TEST(DeviceTests, SetFirmwareAndRun)
{
    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre_pilot::Device device(config);

    // Set the firmware to a lambda that writes "Hello, World!" to the UART
    // buffer
    device.setFirmware(
        [](sabre::core::ResourceManager &rm)
        {
            rm.serial().configureUart(0, 9600, rm.gpio().getGpio(0),
                                      rm.gpio().getGpio(0), 128);
            auto &uart = rm.serial().getUart(0);
            uart.initialize();
            const std::string message = "Hello, World!";
            for (char c : message)
            {
                uart.writeByte(c);
            }
            uart.flush();
            return 0;
        });

    device.run();
    ASSERT_EQ(device.getUartBuffer(0), "Hello, World!");
}

TEST(DeviceTests, RunWithoutFirmware)
{
    sabre::core::ResourceManagerConfig config = {.maxGpios = 1,
                                                 .upperboundUart = 1};
    sabre_pilot::Device device(config);
    device.run();
    ASSERT_EQ(device.getUartBuffer(0), "");
}