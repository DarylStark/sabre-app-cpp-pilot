#include "uart_buffer.hpp"

namespace sabre_pilot
{
    UartBuffer::UartBuffer() : _buffer("") {}

    void UartBuffer::addData(const std::string &data)
    {
        _buffer.append(data);
    }

    const std::string &UartBuffer::getBuffer() const
    {
        return _buffer;
    }
} // namespace sabre_pilot