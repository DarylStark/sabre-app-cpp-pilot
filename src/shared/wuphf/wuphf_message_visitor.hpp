#pragma once

#include "wuphf_message.hpp"
#include <ipc/session.hpp>
#include <memory>

namespace sabre::ipc
{
    class WuphfMessageVisitor
    {
    public:
        virtual ~WuphfMessageVisitor() = default;
        virtual void
        visitClientHello(std::shared_ptr<::ipc::IpcSession> session,
                         ClientHello &message) = 0;
        virtual void visitUartAppend(std::shared_ptr<::ipc::IpcSession> session,
                                     UartAppend &message) = 0;
    };

    class WuphfMessageVisitorAdapter : public WuphfMessageVisitor
    {
    public:
        virtual ~WuphfMessageVisitorAdapter() = default;
        virtual void
        visitClientHello(std::shared_ptr<::ipc::IpcSession> session,
                         ClientHello &message) override
        {
        }
        virtual void visitUartAppend(std::shared_ptr<::ipc::IpcSession> session,
                                     UartAppend &message) override
        {
        }
    };
} // namespace sabre::ipc