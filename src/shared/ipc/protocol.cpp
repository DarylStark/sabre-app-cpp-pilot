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
} // namespace ipc