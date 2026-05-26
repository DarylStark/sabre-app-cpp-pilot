#include "../pilot/device.hpp"
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Uart : public sabre::hal::Serial
    {
    private:
        sabre_pilot::Device *_device;
        size_t _uartIndex;

    public:
        Uart(sabre_pilot::Device *device, size_t uartIndex);
        void initialize() override;
        int writeByte(char data) const override;
        std::string readBytes(size_t maxBytes,
                              sabre::types::MsTime timeoutInMs) override;
        void flush() override;
        void deinitialize() override;

        bool isInitialized() const noexcept;

    protected:
        sabre_pilot::UartController &_getUartController() const;
    };
}; // namespace sabre::impl::pilot