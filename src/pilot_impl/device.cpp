#include "device.hpp"
#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    Device::Device(sabre::core::ResourceManagerConfig config) : _config(config)
    {
        if (config.upperboundUart)
            _uartOuput =
                std::make_unique<UartController[]>(config.upperboundUart);
    }

    UartController &Device::getUartController(size_t uartNumber)
    {
        if (uartNumber >= _config.upperboundUart)
            throw DeviceUartNotConfiguredException("Uart number too big");
        return _uartOuput[uartNumber];
    }

    const sabre::core::ResourceManagerConfig &Device::getConfig() const
    {
        return _config;
    }
} // namespace sabre::impl::pilot
