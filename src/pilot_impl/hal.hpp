#include "device.hpp"
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Uart : public sabre::hal::Serial
    {
    private:
        Device *_device;
        size_t _uartIndex;

    public:
        Uart(Device *device, size_t uartIndex);
        void initialize() override;
        int writeByte(char data) const override;
        std::string readBytes(size_t maxBytes,
                              sabre::types::MsTime timeoutInMs) override;
        void flush() override;
        void deinitialize() override;

        bool isInitialized() const noexcept;

    protected:
        UartController &_getUartController() const;
    };

    class Gpio : public sabre::hal::Gpio
    {
    public:
        Gpio(Device *device, sabre::hal::PinNumber pinNumber);
        void reset();

    private:
        Device *_device;
    };
}; // namespace sabre::impl::pilot