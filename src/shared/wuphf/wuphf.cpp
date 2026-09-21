#include "wuphf.hpp"
#include <algorithm>
#include <iostream>
#include <ipc/byte_order.hpp>
#include <ipc/session.hpp>

namespace sabre::ipc
{
    Wuphf::Wuphf(::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
                 std::size_t bufferSize)
        : IpcProtocol(bufferSize), _queue(queue)
    {
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
                std::unique_ptr<IncomingMessage> msg =
                    std::make_unique<IncomingMessage>();
                msg->message = std::move(*message);
                msg->session = _session;
                _queue.push(std::move(msg));
                return length + 4;
            }
        }
        return 0;
    }

    WuphfServer::WuphfServer(
        ::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
        std::size_t bufferSize)
        : Wuphf(queue, bufferSize)
    {
        _parseMethods[0x0001] = [this]() { return _parseClientHello(); };
        _parseMethods[0x0101] = [this]() { return _parseUartAppend(); };
    }

    std::optional<WuphfMessage::UniquePtr> WuphfServer::_parseClientHello()
    {
        auto rv = ClientHello::deserializeObj(_buffer | std::views::drop(4) |
                                              std::views::take(4));

        if (rv != std::nullopt)
        {
            if (_state != WuphfServerState::Pending)
            {
                // TODO: Custom exception
                throw std::runtime_error(
                    "Server received ClientHello when not in pending state");
            }

            _mcuId = (*rv)->getDestinationMcuId();
            _state = WuphfServerState::Done;

            if (_session)
            {
                ServerHello server_hello(_mcuId);
                sendWuphfMessage(*_session, server_hello);
            }

            return std::make_unique<BindSession>(_mcuId);
        }

        return std::nullopt;
    }

    std::optional<WuphfMessage::UniquePtr> WuphfServer::_parseUartAppend()
    {
        uint16_t length = _deserialize<uint16_t>(2);
        return UartAppend::deserializeObj(
            _mcuId, _buffer | std::views::drop(4) | std::views::take(length));
    }

    WuphfClient::WuphfClient(
        ::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
        std::size_t bufferSize)
        : Wuphf(queue, bufferSize)
    {
        _parseMethods[0x0002] = [this]() { return _parseServerHello(); };
    }

    std::optional<WuphfMessage::UniquePtr> WuphfClient::_parseServerHello()
    {
        auto rv = ServerHello::deserializeObj(_buffer | std::views::drop(4) |
                                              std::views::take(4));

        if (rv != std::nullopt)
        {
            if (_state != WuphfClientState::Pending)
            {
                // TODO: Custom exception
                throw std::runtime_error(
                    "Client received ServerHello when not in pending state");
            }

            _state = WuphfClientState::Done;
            return std::nullopt; // TODO: Something like a IsReadyState or
                                 // something.
        }

        return std::nullopt;
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

    void sendWuphfMessage(::ipc::IpcSession &client,
                          const WuphfMessage &message)
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

        client.send(bytes);
    }
} // namespace sabre::ipc