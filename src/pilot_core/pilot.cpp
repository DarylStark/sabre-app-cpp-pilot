#include "pilot.hpp"
#include "linux_dynamic_library.hpp"
#include <dlfcn.h>
#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/mcu.hpp>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

namespace sabre_pilot
{
    using StartAppFn = void (*)(sabre::core::ResourceManager &);

    Pilot::Pilot()
    {
        _deviceConfig = std::make_unique<sabre::core::ResourceManagerConfig>(
            sabre::core::ResourceManagerConfig{.maxGpios = 26,
                                               .upperboundUart = 3});
    }

    void Pilot::run()
    {
        std::cout << "Sabre Pilot is starting ... \n";
        sabre::core::ResourceManagerConfig config = {.maxGpios = 26,
                                                     .upperboundUart = 3};
        sabre::impl::pilot::Mcu my_device(config);
        sabre::impl::pilot::Factory factory(&my_device);
        sabre::core::ResourceManager rm(factory, config);

        std::string uartOutputBuffer[3];

        // Configure the UART devices for the `device`
        for (uint16_t idx = 0; idx < config.upperboundUart; idx++)
        {
            my_device.getUartController(idx).setOutputBufferCallback(
                [idx, &uartOutputBuffer](char b)
                {
                    uartOutputBuffer[idx].push_back(b);
                    std::cout << "UART" << idx << " --> "
                              << uartOutputBuffer[idx] << '\n';
                });
        }

        LinuxDynamicLibrary app("../app_example_app/libapp_example_app.so");
        app.load();
        app.getEntryPoint("startApp")(rm);
    }
} // namespace sabre_pilot