#include "hal.hpp"

namespace sabre::impl::pilot
{
    Uart::Uart(sabre_pilot::Device *device)
        : _device(device), _initialized(false)
    {
    }

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
} // namespace sabre::impl::pilot