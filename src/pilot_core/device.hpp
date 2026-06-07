#pragma once

#include "dynamic_library.hpp"
#include <functional>
#include <memory>
#include <pilot_impl/core.hpp>
#include <pilot_impl/mcu.hpp>
#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot
{
    using DeviceStartMethod = std::function<void(sabre::core::ResourceManager)>;

    class Device
    {
    private:
        LibraryEntryPoint _firmware;
        sabre::core::ResourceManagerConfig _config;
        sabre::impl::pilot::Mcu _mcu;
        sabre::impl::pilot::Factory _factory;

    public:
        Device(sabre::core::ResourceManagerConfig config);
        void setFirmware(LibraryEntryPoint firmware);
        void run();
    };
} // namespace sabre_pilot