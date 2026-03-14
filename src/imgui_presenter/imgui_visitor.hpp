#pragma once

#include "../simulator/device_visitor.hpp"
#include <unordered_map>

namespace sabre::impl::pilot
{
    class ImGuiVisitor : public DeviceVisitor
    {
    private:
        std::unordered_map<std::string, bool> &_deviceVisibility;

    public:
        ImGuiVisitor(std::unordered_map<std::string, bool> &deviceVisibility);
        ~ImGuiVisitor() = default;
        void visitMcu(class Mcu &mcu, const std::string &name) override;
    };
} // namespace sabre::impl::pilot