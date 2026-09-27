#include <hardware/controller.hpp>
#include <sabre/hal/gpio.hpp>
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Uart : public sabre::hal::Serial
    {
    private:
        size_t _uartIndex;
        sabre_runner::hardware::Controller::SharedPtr _hardware;
        size_t _bufferSize;

    public:
        Uart(sabre_runner::hardware::Controller::SharedPtr hardware,
             sabre::hal::UartNumber uartIndex, size_t bufferSize);
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