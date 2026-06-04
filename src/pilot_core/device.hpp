#pragma once

#include <functional>
#include <memory>
#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot
{
    using DeviceStartMethod = std::function<void(sabre::core::ResourceManager)>;

    class Device
    {
    private:
        sabre::core::ResourceManagerConfig _config;

    public:
        Device(sabre::core::ResourceManagerConfig config);
        void run();
    };
} // namespace sabre_pilot