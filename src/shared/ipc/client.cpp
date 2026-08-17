#include "client.hpp"

namespace ipc
{
    IpcClient::IpcClient(std::shared_ptr<IpcProtocol> protocol)
        : _protocol(std::move(protocol))
    {
    }
} // namespace ipc