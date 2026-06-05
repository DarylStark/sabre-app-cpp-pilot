#pragma once

#include "dynamic_library.hpp"
#include <functional>
#include <memory>
#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot
{
    using DeviceStartMethod = std::function<void(sabre::core::ResourceManager)>;

    class Device
    {
    private:
        LibraryEntryPoint _firmware;
        sabre::core::ResourceManagerConfig _config;

    public:
        Device(sabre::core::ResourceManagerConfig config);
        void setFirmware(LibraryEntryPoint firmware);
        void run();
    };
} // namespace sabre_pilot