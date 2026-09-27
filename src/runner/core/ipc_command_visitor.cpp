#include "ipc_command_visitor.hpp"

#include <iostream>

namespace sabre_runner::core
{
    void IpcCommandVisitor::visitClientConnected(
        sabre::ipc::ClientConnected &message)
    {
        std::cout << "CLIENT IS CONNECTED!\n";
    }
} // namespace sabre_runner::core