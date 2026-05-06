#include "device.hpp"
#include "exceptions.hpp"

namespace sabre_pilot
{
    Device::Device(sabre::core::ResourceManagerConfig config) : _config(config)
    {
        if (config.upperboundUart)
            _uartOuput = std::make_unique<UartBuffer[]>(config.upperboundUart);
    }

    UartBuffer &Device::outputUart(size_t uartNumber)
    {
        if (uartNumber >= _config.upperboundUart)
            throw sabre_pilot::exceptions::DeviceUartNotConfiguredException(
                "Uart number too big");
        return _uartOuput[uartNumber];
    }

    const sabre::core::ResourceManagerConfig &Device::config() const
    {
        return _config;
    }
} // namespace sabre_pilot
