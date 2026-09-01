#include "protocol.hpp"
#include <iostream>
#include <span>

namespace ipc
{
    IpcProtocol::IpcProtocol(std::size_t bufferSize)
    {
        _buffer.reserve(bufferSize);
    }

    void IpcProtocol::pushBytes(std::span<const std::byte> bytes)
    {
        _buffer.insert(_buffer.end(), bytes.begin(), bytes.end());
        parseBuffer();
    }

    void IpcProtocol::parseBuffer()
    {
        while (true)
        {
            size_t parsedBytes = _parseOnePacket();
            if (parsedBytes > 0)
            {
                std::cout << "PROTOCOL: Parsed " << parsedBytes << " bytes!\n";
                _buffer.erase(_buffer.begin(), _buffer.begin() + parsedBytes);
                continue;
            }
            break;
        }
    }

    std::uint16_t IpcProtocol::_readU16_be(std::size_t offset) const
    {
        if (offset + 2 > _buffer.size())
        {
            // TODO: custom exception
            throw std::out_of_range("Not enough bytes to read uint16_t");
        }

        return (static_cast<std::uint16_t>(_buffer[offset]) << 8) |
               (static_cast<std::uint16_t>(_buffer[offset + 1]));
    }

    std::uint32_t IpcProtocol::_readU32_be(std::size_t offset) const
    {
        if (offset + 4 > _buffer.size())
        {
            // TODO: custom exception
            throw std::out_of_range("Not enough bytes to read uint32_t");
        }

        return (static_cast<uint32_t>(_buffer[offset]) << 24) |
               (static_cast<uint32_t>(_buffer[offset + 1]) << 16) |
               (static_cast<uint32_t>(_buffer[offset + 2]) << 8) |
               static_cast<uint32_t>(_buffer[offset + 3]);
    }
} // namespace ipc