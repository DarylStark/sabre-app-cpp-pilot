#include "wuphf.hpp"

namespace sabre_pilot::ipc
{
    ::ipc::IpcProtocol<WuphfCommand::UniquePtr>::UniquePtr Wuphf::clone()
    {
        return std::make_unique<Wuphf>();
    }

    std::optional<WuphfCommand::UniquePtr> Wuphf::_parseClientHello()
    {
        uint16_t length = _readU16_be(2);

        if (length != 4)
        {
            return std::nullopt;
        }

        uint32_t id = _readU32_be(4);
        _mcuId = id;

        // Clear the data from the buffer
        _buffer.erase(_buffer.begin(), _buffer.begin() + 8);

        return std::make_unique<ClientHello>(id);
    }

    std::optional<WuphfCommand::UniquePtr> Wuphf::_parseUartAppend()
    {
        if (!_mcuId)
            return std::nullopt;

        uint16_t length = _readU16_be(2);
        uint16_t dataLength = length - 2;

        if (_buffer.size() < 4 + length)
        {
            return std::nullopt;
        }

        uint16_t uartId = _readU16_be(4);
        std::string data(_buffer.begin() + 6, _buffer.begin() + 6 + dataLength);

        _buffer.erase(_buffer.begin(), _buffer.begin() + 4 + length);

        return std::make_unique<UartAppend>(_mcuId, uartId, data);
    }

    std::uint16_t Wuphf::_readU16_be(std::size_t offset) const
    {
        if (offset + 2 > _buffer.size())
        {
            // TODO: custom exception
            throw std::out_of_range("Not enough bytes to read uint16_t");
        }

        return (static_cast<std::uint16_t>(_buffer[offset]) << 8) |
               (static_cast<std::uint16_t>(_buffer[offset + 1]));
    }

    std::uint32_t Wuphf::_readU32_be(std::size_t offset) const
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

    std::optional<WuphfCommand::UniquePtr>
    Wuphf::parseBytes(std::vector<uint8_t> &bytes)
    {
        _buffer.insert(_buffer.end(), bytes.begin(), bytes.end());

        // Buffer should be at least 4 bytes to process
        if (_buffer.size() < 4)
        {
            return std::nullopt;
        }

        // Get the fields
        uint16_t type = _readU16_be(0);
        uint16_t length = _readU16_be(2);

        // If this is not a full packet, we have to abort
        if (_buffer.size() < 4 + length)
        {
            return std::nullopt;
        }

        if (type == 0x0001)
        {
            return _parseClientHello();
        }

        if (type == 0x0101)
        {
            return _parseUartAppend();
        }

        // Not a valid command
        return std::nullopt;
    }
} // namespace sabre_pilot::ipc