#include "../pilot/device.hpp"
#include <sabre/hal/serial.hpp>

namespace sabre::impl::pilot
{
    class Uart : public sabre::hal::Serial
    {
    private:
        sabre_pilot::Device *_device;
        bool _initialized;

    public:
        Uart(sabre_pilot::Device *device);
        void initialize();
        int writeByte(char data) const;
        std::string readBytes(size_t maxBytes,
                              sabre::types::MsTime timeoutInMs);
        void flush();
        void deinitialize();
    };
}; // namespace sabre::impl::pilot