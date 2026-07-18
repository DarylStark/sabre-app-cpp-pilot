#include "wuphf.hpp"

namespace sabre_pilot::ipc
{
    std::unique_ptr<::ipc::IpcProtocol<std::unique_ptr<WuphfCommand>>>
    Wuphf::clone()
    {
        return std::make_unique<Wuphf>();
    }

    std::optional<std::unique_ptr<WuphfCommand>> Wuphf::_parseClientHello()
    {
        uint16_t length = _readU16_be(2);

        if (length != 4)
        {
            return std::nullopt;
        }

        uint32_t id = _readU32_be(4);
        _mcuId = id;

        return std::make_unique<WuphfCommand>(id);
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

    std::optional<std::unique_ptr<WuphfCommand>>
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

        if (type == 0x01)
        {
            return _parseClientHello();
        }

        if (_mcuId == 0)
            return std::nullopt;
        return std::make_unique<WuphfCommand>(_mcuId);
    }
} // namespace sabre_pilot::ipc