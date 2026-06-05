#pragma once

#include "device.hpp"
#include "dynamic_library.hpp"
#include "pilot_project.hpp"
#include <sabre/core/resource_manager.hpp>
#include <thread>
#include <unordered_map>

namespace sabre_pilot
{
    class Pilot
    {
    private:
        Project _project;
        std::unordered_map<std::string, std::unique_ptr<Device>> _devices;
        std::unordered_map<std::string, std::unique_ptr<DynamicLibrary>>
            _libraries;
        std::vector<std::unique_ptr<std::thread>> _firmwareThreads;

        void _loadProject();

    public:
        Pilot(Project project);
        void run();
    };
} // namespace sabre_pilot