#include "runner.hpp"
#include "exceptions.hpp"
#include "linux_dynamic_library.hpp"
#include <iostream> // TODO: Remove
#include <ipc_tcp/client.hpp>
#include <sabre_impl/core.hpp>
#include <thread>
#include <wuphf/wuphf.hpp>
#include <wuphf/wuphf_client.hpp>
#include <wuphf/wuphf_message.hpp>

namespace sabre_runner::core
{
    IpcModeVisitor::IpcModeVisitor(ipc::IpcProtocol::SharedPtr protocol)
        : _ipcProtocol(std::move(protocol))
    {
    }

    ipc::IpcClient::SharedPtr IpcModeVisitor::operator()(IpcNoneConfig &config)
    {
        return nullptr;
    }

    ipc::IpcClient::SharedPtr IpcModeVisitor::operator()(IpcTcpConfig &config)
    {
        return std::make_shared<ipc::tcp::TcpIpcClient>(
            _ipcProtocol, config.serverIp, config.serverPort);
    }

    Runner::Runner(CoreConfig config)
        : _config(std::move(config)),
          _ipcProtocol(
              std::make_shared<sabre::ipc::WuphfClient>(_ipcQueue, 2048)),
          _library(std::make_unique<LinuxDynamicLibrary>(
              _config.software.firmwareFile))
    {
    }

    void Runner::_ipcReceiverThreadFn()
    {
        bool keepRunning = true;
        while (keepRunning)
        {
            std::optional<std::unique_ptr<sabre::ipc::IncomingMessage>> item =
                _ipcQueue.pop();
            if (item)
            {
                std::unique_ptr<sabre::ipc::IncomingMessage> message =
                    std::move(*item);
                if (message == nullptr)
                    continue;
                if (message->message == nullptr)
                    continue;
                std::cout << "Message received!\n";
            }
            else
            {
                keepRunning = false;
            }
        }
    }

    void Runner::_loadEntryPoint()
    {
        _entryPointFn = _library->getEntryPoint(_config.software.entryPoint);
    }

    void Runner::_startIpc()
    {
        if (!_ipcClient)
            return;

        _ipcClient->setup();
        _ipcClientThread =
            std::make_unique<std::thread>([this]() { _ipcClient->run(); });

        if (!_ipcClient->waitForConnection())
        {
            _ipcClient->stop();
            _ipcClientThread->join();
            throw IpcException("Error connecting to IPC server");
        }

        _ipcClientThread->detach();

        // Run a thread checking the IPC queue
        _ipcReceiverThread =
            std::make_unique<std::thread>([this]() { _ipcReceiverThreadFn(); });
        _ipcReceiverThread->detach();

        // Create a Hello Command
        sabre::ipc::ClientHello hello(_config.deviceId);
        sabre::ipc::sendWuphfMessage(*_ipcClient, hello);
    }

    void Runner::_configureIpc()
    {
        _ipcClient = std::visit(IpcModeVisitor(_ipcProtocol), _config.ipc);
    }

    void Runner::_configureHardware()
    {
        using sabre_runner::hardware::Controller;
        _hardware = std::make_shared<Controller>(_config.hardware, _ipcClient);
    }

    void Runner::_startFirmware()
    {
        if (!_entryPointFn)
        {
            throw NotConfigureException("Entry point not configured.");
        }

        sabre::core::ResourceManagerConfig config;
        config.maxGpios = _config.hardware.maxGpios;
        config.upperboundUart = _config.hardware.upperboundUart;

        sabre::impl::pilot::Factory fac(_hardware);
        sabre::core::ResourceManager rm(fac, config);

        _entryPointFn(rm);
    }

    void Runner::start()
    {
        _loadEntryPoint();
        _configureIpc();
        _configureHardware();
        _startIpc();
        _startFirmware();
    }
} // namespace sabre_runner::core