#include "ipc_command_visitor.hpp"

#include "runner.hpp"

namespace sabre_runner::core
{
    IpcCommandVisitor::IpcCommandVisitor(Runner &runner) : _runner(runner) {}

    void IpcCommandVisitor::visitClientConnected(
        sabre::ipc::ClientConnected &message)
    {
        _runner.markIpcReady();
    }
} // namespace sabre_runner::core