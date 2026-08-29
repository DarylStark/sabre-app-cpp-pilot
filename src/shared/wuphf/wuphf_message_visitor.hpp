#pragma once

#include "wuphf_command.hpp"

namespace sabre::ipc
{
    class WuphfMessageVisitor
    {
    public:
        virtual ~WuphfMessageVisitor() = default;
        virtual void visitClientHello(ClientHello &message) = 0;
        virtual void visitUartAppend(UartAppend &message) = 0;
    };

    class WuphfMessageVisitorAdapter : public WuphfMessageVisitor
    {
    public:
        virtual ~WuphfMessageVisitorAdapter() = default;
        virtual void visitClientHello(ClientHello &message) override {}
        virtual void visitUartAppend(UartAppend &message) override {}
    };
} // namespace sabre::ipc