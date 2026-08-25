#include <sabre/hal/gpio.hpp>
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Uart : public sabre::hal::Serial
    {
    private:
        size_t _uartIndex;

    public:
        Uart(sabre::hal::UartNumber uartIndex);
        void initialize() override;
        int writeByte(char data) const override;
        std::string readBytes(size_t maxBytes,
                              sabre::types::MsTime timeoutInMs) override;
        void flush() override;
        void deinitialize() override;

        bool isInitialized() const noexcept;
    };

    class Gpio : public sabre::hal::Gpio
    {
    public:
        Gpio(sabre::hal::PinNumber pinNumber);
        void reset() override;
    };
}; // namespace sabre::impl::pilot