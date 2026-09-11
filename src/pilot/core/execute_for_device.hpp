#pragma once

#include "device.hpp"
#include <optional>
#include <wuphf/wuphf_message_visitor.hpp>

namespace sabre_pilot::core
{
    class ExecuteForDevice : public sabre::ipc::WuphfMessageVisitorAdapter
    {
    private:
        std::shared_ptr<Device> _device;

    public:
        void setDevice(std::shared_ptr<Device> device);
        void visitClientHello(std::shared_ptr<::ipc::IpcSession> session,
                              sabre::ipc::ClientHello &message) override;
        void visitUartAppend(std::shared_ptr<::ipc::IpcSession> session,
                             sabre::ipc::UartAppend &message) override;
        void visitBindSession(std::shared_ptr<::ipc::IpcSession> session,
                              sabre::ipc::BindSession &message) override;
    };
} // namespace sabre_pilot::core