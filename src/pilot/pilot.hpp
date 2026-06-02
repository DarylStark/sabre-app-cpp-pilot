#pragma once

#include "device.hpp"
#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot
{
    class Pilot
    {
    private:
        std::unique_ptr<Device> _device;
        std::unique_ptr<sabre::core::ResourceManagerConfig> _deviceConfig;

    public:
        Pilot();
        void run();
    };
} // namespace sabre_pilot