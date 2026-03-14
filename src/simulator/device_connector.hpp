#pragma once

#include "device.hpp"

namespace sabre::impl::pilot
{
    class UartConnector
    {
    private:
        Device &_deviceA;
        uint32_t _uartNumberA;
        Device &_deviceB;
        uint32_t _uartNumberB;

    public:
        UartConnector(Device &deviceA, uint32_t uartNumberA, Device &deviceB,
                      uint32_t uartNumberB);
    };
} // namespace sabre::impl::pilot