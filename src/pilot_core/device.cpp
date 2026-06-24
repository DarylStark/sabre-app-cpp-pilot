#include "device.hpp"
#include <iostream>

namespace sabre_pilot
{
    Device::Device(DeviceConfig config) : _config(std::move(config))
    {
        _uartBuffers = std::make_unique<std::string[]>(
            _config.deviceConfig.upperboundUart);
    }

    void Device::start()
    {
        // TODO: Implement (issue 13)
    }

    void Device::stop()
    {
        // TODO: Implement (issue 13)
    }

    const std::string &
    Device::getUartBuffer(sabre::hal::UartNumber uartIdx) const
    {
        return _uartBuffers[uartIdx];
    }

    void Device::clearUartBuffer(sabre::hal::UartNumber uartIdx)
    {
        _uartBuffers[uartIdx].clear();
    }

    sabre::hal::UartNumber Device::getUartCount() const
    {
        return _config.deviceConfig.upperboundUart;
    }
} // namespace sabre_pilot