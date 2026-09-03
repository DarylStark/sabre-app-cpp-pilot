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
        auto rv = ClientHello::deserializeObj(_buffer | std::views::drop(4) |
                                              std::views::take(4));
        if (rv != std::nullopt)
        {
            _mcuId = (*rv)->getDestinationMcuId();
        }
        return rv;
    }

    std::optional<WuphfMessage::UniquePtr> Wuphf::_parseUartAppend()
    {
        uint16_t length = _deserialize<uint16_t>(2);
        return UartAppend::deserializeObj(
            _mcuId, _buffer | std::views::drop(4) | std::views::take(length));
    }

    void sendWuphfMessage(::ipc::IpcClient &client, const WuphfMessage &message)
    {
        const uint16_t opcode = message.getOpCode();
        const auto data = message.serializeObj();

        ::ipc::BufferType bytes(4 + data.size());
        size_t length = data.size();

        // Copy the opcode
        std::ranges::copy(::ipc::byte_order::serialize<uint16_t>(opcode),
                          bytes.begin());

        // Copy the size
        std::ranges::copy(::ipc::byte_order::serialize<uint16_t>(length),
                          bytes.begin() + 2);

        // Copy the data
        std::ranges::copy(data, bytes.begin() + 4);

        client.sendData(bytes);
    }
} // namespace sabre::ipc