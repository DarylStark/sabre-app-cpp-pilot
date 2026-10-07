#pragma once

#include <wuphf/wuphf_message_visitor.hpp>

namespace sabre_runner::core
{
    class Runner;

    class IpcCommandVisitor : public sabre::ipc::WuphfMessageVisitorAdapter
    {
    private:
        Runner &_runner;

    public:
        IpcCommandVisitor(Runner &runner);
        void
        visitClientConnected(sabre::ipc::ClientConnected &message) override;
    };
} // namespace sabre_runner::core