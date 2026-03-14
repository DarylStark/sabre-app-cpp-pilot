#pragma once

#include "../simulator/device.hpp"
#include <sabre/hal/gpio.hpp>
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Serial : public sabre::hal::Serial
    {
    private:
        Device *_device;
        uint32_t _uartNumber;
        size_t _bufferSize;

    public:
        Serial(Device *device, sabre::hal::UartNumber number,
               size_t bufferSize);
        void initialize() override;
        int writeByte(char data) const override;
        std::string readBytes(size_t maxBytes,
                              sabre::types::MsTime timeoutInMs) override;
        void flush() override;
        void deinitialize() override;
    };

    class Gpio : public sabre::hal::Gpio
    {
    private:
        Device *_device;

    public:
        Gpio(Device *device, uint32_t pin);
        void reset() override;
    };

    class InputGpio : public sabre::hal::InputGpio
    {
    private:
        Device *_device;
        uint32_t _pin;

    protected:
        bool _getLevel() const override;

    public:
        InputGpio(Device *device, uint32_t pin);

        void reset() override;
        void enablePullup() override;
        void enablePulldown() override;
        void disablePullup() override;
        void disablePulldown() override;
        void addInterruptHandler(sabre::hal::ISRHandler,
                                 sabre::hal::ISRTrigger) override;
    };

    class OutputGpio : public sabre::hal::OutputGpio
    {
    private:
        Device *_device;
        uint32_t _pin;

    public:
        OutputGpio(Device *device, uint32_t pin);
        void reset() override;
        void setHigh() override;
        void setLow() override;
        void setLevel(bool level) override;
    };
} // namespace sabre::impl::pilot