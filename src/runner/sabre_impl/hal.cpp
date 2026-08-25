#include "hal.hpp"
#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    Uart::Uart(sabre_runner::hardware::Controller::SharedPtr hardware,
               sabre::hal::UartNumber uartIndex, size_t bufferSize)
        : _uartIndex(uartIndex), _hardware(std::move(hardware)),
          _bufferSize(bufferSize)
    {
    }

    void Uart::initialize()
    {
        _hardware->getUartController(_uartIndex)
            .initialize(_bufferSize, _bufferSize);
    }

    int Uart::writeByte(char data) const
    {
        _hardware->getUartController(_uartIndex).write(data);
        return 0;
    }

    std::string Uart::readBytes(size_t maxytes, sabre::types::MsTime timeout)
    {
        return "test"; // TODO: Make something good
    }

    void Uart::flush()
    {
        _hardware->getUartController(_uartIndex).flush();
    }

    void Uart::deinitialize() {}

    bool Uart::isInitialized() const noexcept
    {
        return _hardware->getUartController(_uartIndex).isInitialized();
    }

    Gpio::Gpio(sabre::hal::PinNumber pinNumber) : sabre::hal::Gpio(pinNumber) {}

    void Gpio::reset() {}
} // namespace sabre::impl::pilot