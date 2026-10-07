#include "server.hpp"
#include <algorithm>
#include <iostream>

namespace ipc
{
    IpcServer::~IpcServer()
    {
        _stopAllSessions();
    }

    void IpcServer::_stopAllSessions()
    {
        for (auto &session : _sessions)
        {
            session->stop();
        }
    }

    void
    IpcServer::_removeSession(const std::shared_ptr<::ipc::IpcSession> &session)
    {
        std::erase(_sessions, session);
        std::cout << "SERVER: Session removed. Active sessions: "
                  << _sessions.size() << '\n';
    }

    std::size_t IpcServer::getSessionCount() const
    {
        return _sessions.size();
    }

    void IpcServer::_addSession(std::shared_ptr<IpcSession> session)
    {
        _sessions.push_back(std::move(session));
    }

    IpcSession &IpcServer::getSession(std::size_t index) const
    {
        if (index > _sessions.size())
        {
            throw std::out_of_range("Session not found");
        }
        return *(_sessions[index]);
    }

    std::vector<std::shared_ptr<::ipc::IpcSession>>
    IpcServer::getSessions() const
    {
        return _sessions;
    }
} // namespace ipc