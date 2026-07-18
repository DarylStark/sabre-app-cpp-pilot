#include "pilot.hpp"
#include "device.hpp"
#include "subprocess_strategy.hpp"
#include <iostream>
#include <ipc/tcp/server.hpp>
#include <memory>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>
#include <wuphf/wuphf.hpp>

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

    void Pilot::start()
    {
        using ::ipc::TcpIpcServer;
        using sabre_pilot::ipc::Wuphf;
        using sabre_pilot::ipc::WuphfCommand;

        // Start process monitor
        auto threadLambda = [this]() { this->_processMonitorThreadFn(); };
        _processMonitorThread = std::make_unique<std::thread>(threadLambda);
        _processMonitorThread->detach();

        // IPC server
        Wuphf::SharedPtr protocol = std::make_shared<Wuphf>();

        // Run a thread checking the IPC queue
        std::thread ipcThread(
            [this]()
            {
                bool keepRunning = true;
                while (keepRunning)
                {
                    auto item = _ipc_queue.pop();
                    if (item)
                    {
                        std::cout << "Received message for "
                                  << (*item)->getDestinationMcuId() << "\n";
                    }
                    else
                    {
                        keepRunning = false;
                    }
                }
            });
        ipcThread.detach();

        // TODO: Make the specific concrete IPC server configurable
        _ipcServer = std::make_unique<TcpIpcServer<WuphfCommand::UniquePtr>>(
            protocol, _ipc_queue, 8998);
        _ipcServer->setup();
        _ipcServerThread = std::make_unique<std::thread>(
            [this]() { this->_ipcServer->run(); });
        _ipcServerThread->detach();
    }
} // namespace sabre_pilot