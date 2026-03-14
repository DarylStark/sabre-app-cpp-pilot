#pragma once

#include "device.hpp"
#include "simulator.hpp"
#include <string>

namespace sabre::impl::pilot
{
    class Mcu : public Device
    {
    public:
        Mcu(DeviceConfig config, sabre::runtime::App::UniquePtr &&app);
        void accept(DeviceVisitor &visitor, const std::string &name) override;
    };
} // namespace sabre::impl::pilot