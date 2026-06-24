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

    public:
        Pilot();

        void addDevice(const std::string &name,
                       const sabre::core::ResourceManagerConfig &config,
                       const std::string &library,
                       const std::string &entryPoint);

        const DeviceMap &getDeviceMap() const;

        void startDevice(const std::string &deviceName);
    };
} // namespace sabre_pilot