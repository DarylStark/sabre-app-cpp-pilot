#include "wuphf_server.hpp"
#include "wuphf_message.hpp"
#include <exception>

namespace sabre::ipc
{
    WuphfServer::WuphfServer(::ipc::Queue<WuphfMessage::UniquePtr> &queue,
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

            return std::make_unique<BindSession>(_session, _mcuId);
        }

        return std::nullopt;
    }

    std::optional<WuphfMessage::UniquePtr> WuphfServer::_parseUartAppend()
    {
        uint16_t length = _deserialize<uint16_t>(2);
        return UartAppend::deserializeObj(
            _mcuId, _buffer | std::views::drop(4) | std::views::take(length));
    }
} // namespace sabre::ipc