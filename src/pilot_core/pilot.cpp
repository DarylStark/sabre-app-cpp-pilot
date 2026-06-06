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
    Pilot::Pilot() {}

    void Pilot::addDevice(const std::string &name,
                          const sabre::core::ResourceManagerConfig &config,
                          const std::string &library,
                          const std::string &entryPoint)
    {
        _devices[name] = std::make_unique<Device>(config);
        if (_libraries.find(library) == _libraries.end())
        {
            _libraries[library] =
                std::make_unique<LinuxDynamicLibrary>(library);
        }

        _libraries[library]->addEntryPoint(entryPoint);
        _libraries[library]->load();
        _devices[name]->setFirmware(
            _libraries[library]->getEntryPoint(entryPoint));
    }

    void Pilot::run()
    {
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