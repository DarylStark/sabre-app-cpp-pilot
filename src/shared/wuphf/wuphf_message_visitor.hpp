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
        virtual void visitUartAppend(UartAppend &message) = 0;
        virtual void visitBindSession(BindSession &message) = 0;
    };

    class WuphfMessageVisitorAdapter : public WuphfMessageVisitor
    {
    public:
        virtual ~WuphfMessageVisitorAdapter() = default;
        virtual void visitUartAppend(UartAppend &message) override {}
        virtual void visitBindSession(BindSession &message) {}
    };
} // namespace sabre::ipc