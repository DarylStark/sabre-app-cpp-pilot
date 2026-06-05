#include "pilot.hpp"
#include "device.hpp"
#include "linux_dynamic_library.hpp"
#include <dlfcn.h>
#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/mcu.hpp>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

namespace sabre_pilot
{
    Pilot::Pilot(Project project) : _project(std::move(project)) {}

    void Pilot::_loadProject()
    {
        for (const auto &device : _project.devices)
        {
            _devices[device.name] = std::make_unique<Device>(device.config);
            if (_libraries.find(device.library) == _libraries.end())
            {
                _libraries[device.library] =
                    std::make_unique<LinuxDynamicLibrary>(device.library);
            }
            _libraries[device.library]->addEntryPoint(device.entryPoint);
            _libraries[device.library]->load();
            _devices[device.name]->setFirmware(
                _libraries[device.library]->getEntryPoint(device.entryPoint));
        }
    }

    void Pilot::run()
    {
        _loadProject();

        for (auto &[device_name, device] : _devices)
        {
            std::cout << "Starting device " << device_name << "\n";
            _firmwareThreads.push_back(std::make_unique<std::thread>(
                [dev = device.get()] { dev->run(); }));
        }

        // Join all threads
        for (auto &firmwareThreads : _firmwareThreads)
        {
            firmwareThreads->join();
        }
    }
} // namespace sabre_pilot