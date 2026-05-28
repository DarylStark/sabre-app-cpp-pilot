#include "hal.hpp"
#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    Uart::Uart(Device *device, size_t uartIndex)
        : _device(device), _uartIndex(uartIndex)
    {
        if (!_device)
            throw DeviceNotConfiguredException(
                "A valid pointer to a device is required!");

        try
        {
            _device->getUartController(uartIndex);
        }
        catch (DeviceUartNotConfiguredException &e)
        {
            throw InvalidUartIndex("Invalid Uart index.");
        }
    }

    void Uart::initialize()
    {
        _getUartController().initialize(
            100, 100); // TODO: Make the buffers configurable
    }

    int Uart::writeByte(char data) const
    {
        return _getUartController().write(data);
    }

    std::string Uart::readBytes(size_t maxytes, sabre::types::MsTime timeout)
    {
        return "test"; // TODO: Make something good
    }

    void Uart::flush()
    {
        _getUartController().flush();
    }

    void Uart::deinitialize()
    {
        _getUartController().deinitialize();
    }

    bool Uart::isInitialized() const noexcept
    {
        return _getUartController().isInitialized();
    }

    UartController &Uart::_getUartController() const
    {
        return _device->getUartController(_uartIndex);
    }

    Gpio::Gpio(Device *device, sabre::hal::PinNumber pinNumber)
        : sabre::hal::Gpio(pinNumber), _device(device)
    {
        if (!_device)
            throw DeviceNotConfiguredException(
                "A valid pointer to a device is required!");
    }

    void Gpio::reset() {}
} // namespace sabre::impl::pilot