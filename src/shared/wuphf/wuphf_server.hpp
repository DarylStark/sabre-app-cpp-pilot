#pragma once

#include "wuphf.hpp"
#include "wuphf_message.hpp"
#include <memory>
#include <optional>

namespace sabre::ipc
{
    enum class WuphfServerState
    {
        Pending,
        Done
    };

    class WuphfServer : public Wuphf
    {
    public:
        using Ptr = WuphfServer *;
        using SharedPtr = std::shared_ptr<WuphfServer>;
        using UniquePtr = std::unique_ptr<WuphfServer>;

    private:
        uint32_t _mcuId = 0;

        std::optional<WuphfMessage::UniquePtr> _parseClientHello();
        std::optional<WuphfMessage::UniquePtr> _parseUartAppend();

        WuphfServerState _state = WuphfServerState::Pending;

    public:
        WuphfServer(::ipc::Queue<WuphfMessage::UniquePtr> &queue,
                    std::size_t bufferSize);
    };
} // namespace sabre::ipc