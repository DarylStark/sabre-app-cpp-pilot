#include "wuphf_client.hpp"
#include "wuphf_message.hpp"

namespace sabre::ipc
{
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
} // namespace sabre::ipc