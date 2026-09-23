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
        void visitUartAppend(sabre::ipc::UartAppend &message) override;
        void visitBindSession(sabre::ipc::BindSession &message) override;
    };
} // namespace sabre_pilot::core