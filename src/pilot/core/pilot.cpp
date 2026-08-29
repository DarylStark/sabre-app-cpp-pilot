#include "pilot.hpp"
#include "device.hpp"
#include "execute_for_device.hpp"
#include "subprocess_strategy.hpp"
#include <iostream>
#include <ipc_tcp/server.hpp>
#include <memory>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>
#include <wuphf/wuphf.hpp>

namespace sabre_pilot::core
{
    Pilot::Pilot(const SubprocessStrategy &subprocessStrategy,
                 const std::string &runnerExec)
        : _subprocessStrategy(subprocessStrategy), _runnerExec(runnerExec),
          _nextDeviceId(0)
    {
    }

    Device::DeviceId Pilot::_getNextDeviceId()
    {
        return ++_nextDeviceId;
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
        Device::DeviceId id = _getNextDeviceId();
        _devices[name] = std::make_shared<Device>(
            id, deviceConfig, _subprocessStrategy, _runnerExec);
        _devicesById[id] = _devices[name];
    }

    void Pilot::startDevice(const std::string &deviceName)
    {
        auto device = getDevice(deviceName);
        if (device)
        {
            (*device)->start();
            return;
        }

        // TODO: Proper exception
        std::cerr << "Device " << deviceName << " not found.\n";
        return;
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

    void Pilot::_processMonitorThreadFn()
    {
        using namespace std::chrono_literals;

        std::cout << "Starting Process Monitor\n";
        while (true)
        {
            for (auto &[_, device] : this->_devices)
            {
                device->updateState();
            }
            std::this_thread::sleep_for(100ms); // TODO: make this configurable.
        }
    }

    std::optional<std::shared_ptr<Device>>
    Pilot::getDevice(const std::string &deviceName) const
    {
        auto it = _devices.find(deviceName);
        if (it != _devices.end())
        {
            auto &device = it->second;
            return device;
        }
        return std::nullopt;
    }

    std::optional<std::shared_ptr<Device>>
    Pilot::getDevice(const Device::DeviceId deviceId) const
    {
        auto it = _devicesById.find(deviceId);
        if (it != _devicesById.end())
        {
            auto &device = it->second;
            return device;
        }
        return std::nullopt;
    }

    void Pilot::start()
    {
        using ::ipc::tcp::TcpIpcServer;
        using sabre::ipc::Wuphf;
        using sabre::ipc::WuphfCommand;

        // Start process monitor
        auto threadLambda = [this]() { this->_processMonitorThreadFn(); };
        _processMonitorThread = std::make_unique<std::thread>(threadLambda);
        _processMonitorThread->detach();

        // Run a thread checking the IPC queue
        std::thread ipcThread(
            [this]()
            {
                ExecuteForDevice executor;
                bool keepRunning = true;
                while (keepRunning)
                {
                    std::optional<WuphfCommand::UniquePtr> item =
                        _ipcQueue.pop();
                    if (item)
                    {
                        WuphfCommand::UniquePtr command = std::move(*item);
                        auto device = getDevice(command->getDestinationMcuId());
                        if (device)
                        {
                            command->accept(executor);
                        }
                    }
                    else
                    {
                        keepRunning = false;
                    }
                }
            });
        ipcThread.detach();

        // TODO: Make the specific concrete IPC server configurable
        _ipcServer = std::make_unique<TcpIpcServer>(
            [this]() { return std::make_unique<Wuphf>(_ipcQueue, 4096); },
            8998);

        _ipcServer->setup();
        _ipcServerThread = std::make_unique<std::thread>(
            [this]() { this->_ipcServer->run(); });
        _ipcServerThread->detach();
    }

    void Pilot::stop()
    {
        _ipcServer->stop();
        _ipcQueue.shutdown();
    }
} // namespace sabre_pilot::core