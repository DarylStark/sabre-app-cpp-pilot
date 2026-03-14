#pragma once

#include <string>

namespace sabre::impl::pilot
{
    class DeviceVisitor
    {
    public:
        virtual ~DeviceVisitor() = default;
        virtual void visitMcu(class Mcu &mcu, const std::string &name) = 0;
    };
} // namespace sabre::impl::pilot