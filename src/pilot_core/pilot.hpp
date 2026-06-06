#pragma once

#include "device.hpp"
#include "dynamic_library.hpp"
#include <sabre/core/resource_manager.hpp>
#include <thread>
#include <unordered_map>

namespace sabre_pilot
{
    class Pilot
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Device>> _devices;
        std::unordered_map<std::string, std::unique_ptr<DynamicLibrary>>
            _libraries;
        std::vector<std::unique_ptr<std::thread>> _firmwareThreads;

    public:
        Pilot();

        void addDevice(const std::string &name,
                       const sabre::core::ResourceManagerConfig &config,
                       const std::string &library,
                       const std::string &entryPoint);

        void run();
    };
} // namespace sabre_pilot