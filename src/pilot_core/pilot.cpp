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
        auto device = std::make_unique<Device>(config);

        _devices.insert_or_assign(name, PilotDevice{.name = name,
                                                    .device = std::move(device),
                                                    .library = library,
                                                    .entryPoint = entryPoint});

        auto [libraryIt, inserted] = _libraries.try_emplace(
            library, std::make_unique<LinuxDynamicLibrary>(library));
        libraryIt->second->addEntryPoint(entryPoint);
    }

    void Pilot::run()
    {
        for (auto &[device_name, device] : _devices)
        {
            _libraries[device.library]->load();
            device.device->setFirmware(
                _libraries[device.library]->getEntryPoint(device.entryPoint));

            std::cout << "Starting device " << device_name << "\n";
            device._threadPtr = std::make_unique<std::thread>(
                [dev = device.device.get()] { dev->run(); });
        }

        while (true)
        {
            for (auto &[device_name, device] : _devices)
            {
                for (sabre::hal::UartNumber idx = 0;
                     idx < device.device->getUartCount(); idx++)
                {
                    std::cout << std::string(80, '-') << '\n';
                    std::cout << "DEVICE " << device_name << ", UART " << idx
                              << "\n";
                    std::cout << std::string(80, '-') << '\n';
                    std::cout << device.device->getUartBuffer(idx) << "\n";
                }
            }
        }

        // Join all running threads
        for (auto &[device_name, device] : _devices)
        {
            if (device._threadPtr)
            {
                device._threadPtr->join();
                device._threadPtr.reset();
            }
        }
    }

    const LibraryMap &Pilot::getLibraryMap() const
    {
        return _libraries;
    }

    const DeviceMap &Pilot::getDeviceMap() const
    {
        return _devices;
    }
} // namespace sabre_pilot