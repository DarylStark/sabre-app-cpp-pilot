#include "device.hpp"

namespace sabre_pilot
{

    Device::Device()
    {
        _uartOuput = std::make_unique<UartBuffer[]>(3);
    }

    UartBuffer &Device::outputUart()
    {
        return _uartOuput[0];
    }
} // namespace sabre_pilot
