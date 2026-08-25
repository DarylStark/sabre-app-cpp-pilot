#include "hal.hpp"
#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    Uart::Uart(sabre::hal::UartNumber uartIndex) : _uartIndex(uartIndex) {}

    void Uart::initialize() {}

    int Uart::writeByte(char data) const
    {
        return 0;
    }

    std::string Uart::readBytes(size_t maxytes, sabre::types::MsTime timeout)
    {
        return "test"; // TODO: Make something good
    }

    void Uart::flush() {}

    void Uart::deinitialize() {}

    bool Uart::isInitialized() const noexcept
    {
        return false;
    }

    Gpio::Gpio(sabre::hal::PinNumber pinNumber) : sabre::hal::Gpio(pinNumber) {}

    void Gpio::reset() {}
} // namespace sabre::impl::pilot