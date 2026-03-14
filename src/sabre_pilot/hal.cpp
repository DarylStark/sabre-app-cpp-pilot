#include "hal.hpp"

namespace sabre::impl::pilot
{
    Serial::Serial(Device *device, sabre::hal::UartNumber number,
                   size_t bufferSize)
        : _device(device), _uartNumber(number), _bufferSize(bufferSize)
    {
    }

    void Serial::initialize()
    {
        // TODO : Custom exception
        if (!_device->initialize_uart(_uartNumber, _bufferSize))
            throw std::runtime_error("UART already initialized");
    }

    int Serial::writeByte(char data) const
    {
        if (!_device->write_uart_data(_uartNumber, data))
            return -1; // UART not initialized
        return 0;      // Success
    }

    std::string Serial::readBytes(size_t maxBytes,
                                  sabre::types::MsTime timeoutInMs)
    {
        return _device->read_uart_data(_uartNumber, maxBytes, timeoutInMs);
    }

    void Serial::flush()
    {
        // Implementation for flushing the UART output buffer
        // TODO: Implement something for this
        asm("nop");
    }

    void Serial::deinitialize()
    {
        if (!_device->deinitialize_uart(_uartNumber))
            throw std::runtime_error("UART not initialized");
    }

    Gpio::Gpio(Device *device, uint32_t pin)
        : _device(device), sabre::hal::Gpio(pin)
    {
        _device->set_gpio_type(pin, GPIOType::GENERIC);
    }

    void Gpio::reset()
    {
        // Implementation specific code to reset the GPIO
        _device->reset_gpio(getPinNumber());
    }

    InputGpio::InputGpio(Device *device, uint32_t pin)
        : _device(device), _pin(pin)
    {
        _device->set_gpio_type(pin, GPIOType::INPUT);
    }

    bool InputGpio::_getLevel() const
    {
        // Implementation specific code to get the GPIO level
        return _device->get_gpio(_pin).state > 0;
    }

    void InputGpio::reset()
    {
        // Implementation specific code to reset the GPIO
        _device->reset_gpio(_pin);
    }

    void InputGpio::enablePullup()
    {
        // Implementation specific code to enable pull-up resistor
    }

    void InputGpio::enablePulldown()
    {
        // Implementation specific code to enable pull-down resistor
    }

    void InputGpio::disablePullup()
    {
        // Implementation specific code to disable pull-up resistor
    }

    void InputGpio::disablePulldown()
    {
        // Implementation specific code to disable pull-down resistor
    }

    void InputGpio::addInterruptHandler(sabre::hal::ISRHandler handler,
                                        sabre::hal::ISRTrigger trigger)
    {
        // Implementation specific code to add an interrupt handler
    }

    OutputGpio::OutputGpio(Device *device, uint32_t pin)
        : _device(device), _pin(pin)
    {
        _device->set_gpio_type(pin, GPIOType::OUTPUT);
    }

    void OutputGpio::reset()
    {
        _device->reset_gpio(_pin);
    }

    void OutputGpio::setHigh()
    {
        setLevel(true);
    }

    void OutputGpio::setLow()
    {
        setLevel(false);
    }

    void OutputGpio::setLevel(bool level)
    {
        _device->set_gpio_state(_pin, level ? 1 : 0);
    }
} // namespace sabre::impl::pilot