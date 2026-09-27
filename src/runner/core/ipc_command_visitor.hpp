#pragma once

#include <wuphf/wuphf_message_visitor.hpp>

namespace sabre_runner::core
{
    class IpcCommandVisitor : public sabre::ipc::WuphfMessageVisitorAdapter
    {
    public:
        void
        visitClientConnected(sabre::ipc::ClientConnected &message) override;
    };
} // namespace sabre_runner::core