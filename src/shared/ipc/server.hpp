#pragma once

#include "protocol.hpp"
#include "queue.hpp"
#include "session.hpp"
#include <functional>
#include <memory>

namespace ipc
{
    using ProtocolFactory = std::function<std::unique_ptr<IpcProtocol>(
        std::shared_ptr<IpcSession>)>;

    class IpcServer
    {
    public:
        using Ptr = IpcServer *;
        using SharedPtr = std::shared_ptr<IpcServer>;
        using UniquePtr = std::unique_ptr<IpcServer>;

    protected:
        ProtocolFactory _protocolFactory{};
        std::vector<std::shared_ptr<::ipc::IpcSession>> _sessions;

        // Session management
        void _removeSession(const std::shared_ptr<::ipc::IpcSession> &session);
        void _addSession(std::shared_ptr<IpcSession> session);
        void _stopAllSessions();

    public:
        IpcServer(ProtocolFactory protocolFactory)
            : _protocolFactory(protocolFactory)
        {
        }

        virtual ~IpcServer();

        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;

        // Session retrieval
        std::size_t getSessionCount() const;
        IpcSession &getSession(std::size_t index) const;
        std::vector<std::shared_ptr<::ipc::IpcSession>> getSessions() const;
    };
} // namespace ipc