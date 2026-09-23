#include "session.hpp"
#include "protocol.hpp"
#include <memory>

namespace ipc
{
    void IpcSession::setProtocol(std::unique_ptr<IpcProtocol> protocol)
    {
        _protocol = std::move(protocol);
    }
} // namespace ipc