#include "device.hpp"
#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/mcu.hpp>

namespace sabre_pilot
{
    Device::Device(sabre::core::ResourceManagerConfig config) : _config(config)
    {
    }

    void Device::setFirmware(LibraryEntryPoint firmware)
    {
        _firmware = std::move(firmware);
    }

    void Device::run()
    {
        sabre::impl::pilot::Mcu my_device(_config);
        sabre::impl::pilot::Factory factory(&my_device);
        sabre::core::ResourceManager rm(factory, _config);

        std::string uartOutputBuffer[3];

        // Configure the UART devices for the `device`
        for (uint16_t idx = 0; idx < _config.upperboundUart; idx++)
        {
            my_device.getUartController(idx).setOutputBufferCallback(
                [idx, &uartOutputBuffer](char b)
                {
                    uartOutputBuffer[idx].push_back(b);
                    if (b == '\n')
                    {
                        std::cout << "UART" << idx << " --> "
                                  << uartOutputBuffer[idx];
                        uartOutputBuffer[idx] = "";
                    }
                });
        }

        if (_firmware)
            _firmware(rm);
    }
} // namespace sabre_pilot