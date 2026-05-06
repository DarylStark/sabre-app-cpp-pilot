#pragma once
#include "uart_buffer.hpp"
#include <memory>
#include <string>

namespace sabre_pilot
{
    class Device
    {
    private:
        std::unique_ptr<UartBuffer[]> _uartOuput;

    public:
        Device();

        UartBuffer &outputUart();
    };
} // namespace sabre_pilot