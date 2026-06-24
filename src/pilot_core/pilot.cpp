#include "pilot.hpp"
#include "device.hpp"
#include "subprocess_strategy.hpp"
#include <iostream>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

namespace sabre_pilot
{
    Pilot::Pilot(const SubprocessStrategy &subprocessStrategy,
                 const std::string &runnerExec)
        : _subprocessStrategy(subprocessStrategy), _runnerExec(runnerExec)
    {
    }

    void Pilot::addDevice(const std::string &name,
                          const sabre::core::ResourceManagerConfig &config,
                          const std::string &library,
                          const std::string &entryPoint)
    {
        if (_devices.find(name) != _devices.end())
        {
            // TODO: Proper exception
            std::cerr << "Device " << name << " already exists.\n";
            return;
        }

        DeviceConfig deviceConfig{config, library, entryPoint};
        _devices[name] = std::make_unique<Device>(
            deviceConfig, _subprocessStrategy, _runnerExec);
    }

    void Pilot::startDevice(const std::string &deviceName)
    {
        auto it = _devices.find(deviceName);
        if (it == _devices.end())
        {
            // TODO: Proper exception
            std::cerr << "Device " << deviceName << " not found.\n";
            return;
        }

        auto &device = it->second;
        device->start();
    }

    void Pilot::stopDevice(const std::string &deviceName)
    {
        auto it = _devices.find(deviceName);
        if (it == _devices.end())
        {
            // TODO: Proper exception
            std::cerr << "Device " << deviceName << " not found.\n";
            return;
        }

        auto &device = it->second;
        device->stop();
    }

    const DeviceMap &Pilot::getDeviceMap() const
    {
        return _devices;
    }
} // namespace sabre_pilot