#pragma once

#include "device.hpp"
#include "dynamic_library.hpp"
#include <sabre/core/resource_manager.hpp>
#include <thread>
#include <unordered_map>

namespace sabre_pilot
{
    struct PilotDevice
    {
        std::string name;
        std::unique_ptr<Device> device;
        std::string library;
        std::string entryPoint;
        std::unique_ptr<std::thread> _threadPtr = nullptr;
    };

    class Pilot
    {
    private:
        std::unordered_map<std::string, PilotDevice> _devices;
        std::unordered_map<std::string, std::unique_ptr<DynamicLibrary>>
            _libraries;

    public:
        Pilot();

        void addDevice(const std::string &name,
                       const sabre::core::ResourceManagerConfig &config,
                       const std::string &library,
                       const std::string &entryPoint);

        void run();
    };
} // namespace sabre_pilot