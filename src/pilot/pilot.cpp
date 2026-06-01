#include "pilot.hpp"
#include <app_example_app/example_app.hpp>
#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/mcu.hpp>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

namespace sabre_pilot
{
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

        sabre::runtime::RunApp<MyApp>(rm);
    }
} // namespace sabre_pilot