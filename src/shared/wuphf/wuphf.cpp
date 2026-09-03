#include "wuphf.hpp"
#include <algorithm>
#include <iostream>
#include <ipc/byte_order.hpp>

namespace sabre::ipc
{
    Wuphf::Wuphf(::ipc::Queue<WuphfMessage::UniquePtr> &queue,
                 std::size_t bufferSize)
        : IpcProtocol(bufferSize), _queue(queue)
    {
        _parseMethods[0x0001] = [this]() { return _parseClientHello(); };
        _parseMethods[0x0101] = [this]() { return _parseUartAppend(); };
    }

    std::size_t Wuphf::_parseOnePacket()
    {
        // Buffer should be at least 4 bytes to process
        if (_buffer.size() < 4)
        {
            return 0;
        }

        // Get the fields
        uint16_t type = _deserialize<uint16_t>(0);
        uint16_t length = _deserialize<uint16_t>(2);

        // If this is not a full packet, we have to abort
        if (_buffer.size() < 4 + length)
        {
            return 0;
        }

        const auto method = _parseMethods.find(type);
        if (method != _parseMethods.end())
        {
            std::optional<WuphfMessage::UniquePtr> message;
            message = method->second();
            if (message)
            {
                _queue.push(std::move(*message));
                return length + 4;
            }
        }
        return 0;
    }

    std::optional<WuphfMessage::UniquePtr> Wuphf::_parseClientHello()
    {
        using namespace ::ipc::byte_order;

        uint16_t length = _deserialize<uint16_t>(2);

        if (length != 4)
        {
            return std::nullopt;
        }

        uint32_t id = _deserialize<uint32_t>(4);
        _mcuId = id;

        return std::make_unique<ClientHello>(id);
    }

    std::optional<WuphfMessage::UniquePtr> Wuphf::_parseUartAppend()
    {
        using namespace ::ipc::byte_order;

        if (!_mcuId)
            return std::nullopt;

        uint16_t length = _deserialize<uint16_t>(2);
        uint16_t dataLength = length - 2;

        if (_buffer.size() < 4 + length)
        {
            return std::nullopt;
        }

        uint16_t uartId = _deserialize<uint16_t>(4);
        std::string data;
        data.reserve(dataLength);
        std::transform(_buffer.begin() + 6, _buffer.begin() + 6 + dataLength,
                       std::back_inserter(data),
                       [](std::byte b) { return static_cast<char>(b); });

        return std::make_unique<UartAppend>(_mcuId, uartId, data);
    }

    void sendWuphfMessage(::ipc::IpcClient &client, const WuphfMessage &message)
    {
        const uint16_t opcode = message.getOpCode();
        const auto data = message.getRawBytes();

        ::ipc::BufferType bytes(2 + 2 + data.size());
        size_t length = data.size();

        bytes[0] = static_cast<std::byte>((opcode >> 8) & 0xFF);
        bytes[1] = static_cast<std::byte>(opcode & 0xFF);

        // TODO: check if length is smaller then 2^16

        bytes[2] = static_cast<std::byte>((length & 0xff00) >> 8);
        bytes[3] = static_cast<std::byte>(length & 0x00ff);

        std::copy(data.begin(), data.end(), bytes.begin() + 4);

        client.sendData(bytes);
    }
} // namespace sabre::ipc