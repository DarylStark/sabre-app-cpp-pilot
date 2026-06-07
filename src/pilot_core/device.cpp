#include "device.hpp"
#include <iostream>
#include <pilot_impl/core.hpp>

namespace sabre_pilot
{
    Device::Device(sabre::core::ResourceManagerConfig config)
        : _config(config), _mcu(_config), _factory(&_mcu)
    {
        _uartBuffers = std::make_unique<std::string[]>(config.upperboundUart);

        // Configure the UART devices for the `device`
        for (uint16_t idx = 0; idx < _config.upperboundUart; idx++)
        {
            _mcu.getUartController(idx).setOutputBufferCallback(
                [this, idx](char b) { this->_uartBuffers[idx].push_back(b); });
        }
    }

    void Device::setFirmware(LibraryEntryPoint firmware)
    {
        _firmware = std::move(firmware);
    }

    void Device::run()
    {
        sabre::core::ResourceManager rm(_factory, _config);
        if (_firmware)
            _firmware(rm);

        // Add newlines to the UART buffer
        for (sabre::hal::UartNumber idx = 0; idx < getUartCount(); idx++)
        {
            _uartBuffers[idx].push_back('\n');
        }
    }

    const std::string &
    Device::getUartBuffer(sabre::hal::UartNumber uartIdx) const
    {
        return _uartBuffers[uartIdx];
    }

    sabre::hal::UartNumber Device::getUartCount() const
    {
        return _config.upperboundUart;
    }
} // namespace sabre_pilot