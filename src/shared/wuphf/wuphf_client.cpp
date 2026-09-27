#include "wuphf_client.hpp"
#include "wuphf_message.hpp"

namespace sabre::ipc
{
    WuphfClient::WuphfClient(::ipc::Queue<WuphfMessage::UniquePtr> &queue,
                             std::size_t bufferSize)
        : Wuphf(queue, bufferSize)
    {
        _parseMethods[0x0002] = [this]() { return _parseServerHello(); };
    }

    void WuphfClient::_raiseWhenInWrongState(WuphfClientState expectedState,
                                             std::string_view error) const
    {
        if (_state != expectedState)
        {
            // TODO: Custom exception
            throw std::runtime_error(
                std::string("The server is the wrong state: ") +
                std::string(error));
        }
    }

    void WuphfClient::_raiseWhenNotPending() const
    {
        _raiseWhenInWrongState(WuphfClientState::Pending,
                               "Client should be in pending state.");
    }
    void WuphfClient::_raiseWhenNotDone() const
    {
        _raiseWhenInWrongState(WuphfClientState::Done,
                               "Client should be in done state.");
    }

    std::optional<WuphfMessage::UniquePtr> WuphfClient::_parseServerHello()
    {
        auto rv = ServerHello::deserializeObj(_buffer | std::views::drop(4) |
                                              std::views::take(4));

        if (rv != std::nullopt)
        {
            _raiseWhenNotPending();

            _state = WuphfClientState::Done;
            return std::make_unique<ClientConnected>();
        }

        return std::nullopt;
    }
} // namespace sabre::ipc