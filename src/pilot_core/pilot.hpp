#pragma once

#include "device.hpp"
#include <thread>
#include <unordered_map>

namespace sabre_pilot
{
    using DeviceMap = std::unordered_map<std::string, std::unique_ptr<Device>>;

    class Pilot
    {
    private:
        DeviceMap _devices;
        const SubprocessStrategy &_subprocessStrategy;
        const std::string _runnerExec;

        std::unique_ptr<std::thread> _processMonitorThread = nullptr;

        void _processMonitorThreadFn();

    public:
        Pilot(const SubprocessStrategy &subprocessStrategy,
              const std::string &runnerExec);

        void addDevice(const std::string &name,
                       const sabre::core::ResourceManagerConfig &config,
                       const std::string &library,
                       const std::string &entryPoint);

        const DeviceMap &getDeviceMap() const;

        void startDevice(const std::string &deviceName);
        void stopDevice(const std::string &deviceName);

        void start();
    };
} // namespace sabre_pilot