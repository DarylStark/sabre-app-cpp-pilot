#pragma once

#include "queue.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace ipc
{
    template <typename MessageType>
    class IpcProtocol
    {
    public:
        using Ptr = IpcProtocol<MessageType> *;
        using SharedPtr = std::shared_ptr<IpcProtocol<MessageType>>;
        using UniquePtr = std::unique_ptr<IpcProtocol<MessageType>>;
        Queue<MessageType> &_queue;

    public:
        IpcProtocol(Queue<MessageType> &queue);
        virtual ~IpcProtocol() = default;
        virtual std::unique_ptr<IpcProtocol<MessageType>> clone() = 0;
        virtual void parseBytes(std::vector<uint8_t> &bytes) = 0;
    };

    template <typename MessageType>
    IpcProtocol<MessageType>::IpcProtocol(Queue<MessageType> &queue)
        : _queue(queue)
    {
    }
} // namespace ipc