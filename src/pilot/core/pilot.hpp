#pragma once

#include "device.hpp"
#include <ipc/queue.hpp>
#include <ipc/server.hpp>
#include <memory>
#include <optional>
#include <queue>
#include <thread>
#include <unordered_map>
#include <wuphf/wuphf_command.hpp>

namespace sabre_pilot::core
{
    using DeviceMap = std::unordered_map<std::string, std::shared_ptr<Device>>;
    using DeviceMapId =
        std::unordered_map<Device::DeviceId, std::shared_ptr<Device>>;

    class Pilot
    {
    private:
        DeviceMap _devices;
        DeviceMapId _devicesById;
        const SubprocessStrategy &_subprocessStrategy;
        const std::string _runnerExec;

        ::ipc::IpcServer::UniquePtr _ipcServer;
        std::unique_ptr<std::thread> _processMonitorThread = nullptr;
        std::unique_ptr<std::thread> _ipcServerThread = nullptr;

        ::ipc::Queue<std::unique_ptr<sabre::ipc::WuphfCommand>> _ipcQueue;

        void _processMonitorThreadFn();

        Device::DeviceId _nextDeviceId;
        Device::DeviceId _getNextDeviceId();

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

        std::optional<std::shared_ptr<Device>>
        getDevice(const std::string &deviceName) const;
        std::optional<std::shared_ptr<Device>>
        getDevice(const Device::DeviceId deviceId) const;

        void start();
        void stop();
    };
} // namespace sabre_pilot::core