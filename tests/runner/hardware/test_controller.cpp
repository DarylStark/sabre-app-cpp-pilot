#include <gtest/gtest.h>
#include <hardware/controller.hpp>
#include <hardware/exceptions.hpp>

using namespace sabre_runner::hardware;

TEST(RunnerHardware, RetrieveValidUartControllers)
{
    sabre_runner::core::HardwareConfig config{.maxGpios = 1,
                                              .upperboundUart = 3};
    Controller controller(config, nullptr);

    // Should never give errors
    controller.getUartController(0);
    controller.getUartController(1);
    controller.getUartController(2);
}

TEST(RunnerHardware, RetrieveInvalidUartController)
{
    sabre_runner::core::HardwareConfig config{.maxGpios = 1,
                                              .upperboundUart = 3};
    Controller controller(config, nullptr);

    // Should raise an exception
    ASSERT_THROW(controller.getUartController(4),
                 DeviceUartNotConfiguredException);
}