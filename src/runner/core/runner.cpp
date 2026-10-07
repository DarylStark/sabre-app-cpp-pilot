#include "runner.hpp"
#include "exceptions.hpp"
#include "ipc_command_visitor.hpp"
#include "linux_dynamic_library.hpp"
#include <ipc_tcp/client.hpp>
#include <logger_factory/logger_factory.hpp>
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

    Runner::Runner(CoreConfig config,
                   sabre_logger_factory::LoggerFactory &loggerFactory)
        : _loggerFactory(loggerFactory), _config(std::move(config)),
          _ipcProtocol(
              std::make_shared<sabre::ipc::WuphfClient>(_ipcQueue, 2048)),
          _library(std::make_unique<LinuxDynamicLibrary>(
              _config.software.firmwareFile))
    {
    }

    void Runner::_ipcReceiverThreadFn()
    {
        bool keepRunning = true;

        IpcCommandVisitor visitor(*this);

        while (keepRunning)
        {
            std::optional<sabre::ipc::WuphfMessage::UniquePtr> item =
                _ipcQueue.pop();
            if (item)
            {
                sabre::ipc::WuphfMessage::UniquePtr message = std::move(*item);
                if (message == nullptr)
                    continue;
                message->accept(visitor);
            }
            else
            {
                keepRunning = false;
            }
        }
    }

    void Runner::_loadEntryPoint()
    {
        _logger->info("Loading entrypoint: \"{}\"",
                      _config.software.entryPoint);
        _entryPointFn = _library->getEntryPoint(_config.software.entryPoint);
    }

    void Runner::_startIpc()
    {
        _logger->info("Starting IPC");
        if (!_ipcClient)
            return;

        _ipcClient->setup();
        _ipcClientThread =
            std::make_unique<std::thread>([this]() { _ipcClient->run(); });

        _logger->info("Waiting for IPC connection");
        if (!_ipcClient->waitForConnection())
        {
            _logger->error("Error connecting to IPC server");
            _ipcClient->stop();
            _ipcClientThread->join();
            throw IpcException("Not connected to IPC server");
        }

        _logger->info("Connected!");
        _ipcClientThread->detach();

        // Run a thread checking the IPC queue
        _logger->info("Starting IPC Receiver thread");
        _ipcReceiverThread =
            std::make_unique<std::thread>([this]() { _ipcReceiverThreadFn(); });
        _ipcReceiverThread->detach();

        // Create a Hello Command
        _logger->info("Sending ClientHello to server");
        sabre::ipc::ClientHello hello(_config.deviceId);
        sabre::ipc::sendWuphfMessage(*_ipcClient, hello);
    }

    void Runner::_configureIpc()
    {
        _logger->info("Configuring IPC");
        _ipcClient = std::visit(IpcModeVisitor(_ipcProtocol), _config.ipc);
    }

    void Runner::_configureHardware()
    {
        using sabre_runner::hardware::Controller;
        _logger->info("Configuring hardware");
        _hardware = std::make_shared<Controller>(_config.hardware, _ipcClient);
    }

    void Runner::_startFirmware()
    {
        _logger->info("Configuring firmware");
        if (!_entryPointFn)
        {
            _logger->error("Entry point is not configured correctly");
            throw NotConfigureException("Entry point not configured.");
        }

        sabre::core::ResourceManagerConfig config;
        config.maxGpios = _config.hardware.maxGpios;
        config.upperboundUart = _config.hardware.upperboundUart;

        sabre::impl::pilot::Factory fac(_hardware);
        sabre::core::ResourceManager rm(fac, config);

        _logger->info("Starting firmware");
        _entryPointFn(rm);
    }

    void Runner::_waitForIpcDone()
    {
        _logger->info("Waiting till server has send a ServerHello.");
        std::unique_lock<std::mutex> lock(_ipcReadyMutex);
        _ipcReadyCv.wait(lock);
    }

    void Runner::start()
    {
        _logger = _loggerFactory.make("runner");

        _loadEntryPoint();

        _configureIpc();
        _startIpc();

        _waitForIpcDone();

        _configureHardware();
        _startFirmware();
    }

    void Runner::markIpcReady()
    {
        {
            std::lock_guard<std::mutex> lock(_ipcReadyMutex);
            _ipcReady = true;
        }
        _ipcReadyCv.notify_one();
    }
} // namespace sabre_runner::core