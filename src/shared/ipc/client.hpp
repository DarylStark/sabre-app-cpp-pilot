#pragma once

#include "protocol.hpp"
#include "types.hpp"
#include <memory>

namespace ipc
{
    class IpcClient
    {
    public:
        using Ptr = IpcClient *;
        using SharedPtr = std::shared_ptr<IpcClient>;
        using UniquePtr = std::unique_ptr<IpcClient>;

    protected:
        std::shared_ptr<IpcProtocol> _protocol;

    public:
        IpcClient(std::shared_ptr<IpcProtocol> protocol);

        virtual ~IpcClient() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;

        virtual bool waitForConnection() = 0;
        virtual void sendData(const BufferType &data) = 0;
    };
} // namespace ipc