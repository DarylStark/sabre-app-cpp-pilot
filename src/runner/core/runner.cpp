#include "runner.hpp"
#include "exceptions.hpp"
#include "linux_dynamic_library.hpp"
#include <ipc_tcp/client.hpp>
#include <sabre_impl/core.hpp>
#include <thread>
#include <wuphf/wuphf.hpp>

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
          _ipcProtocol(std::make_shared<sabre::ipc::Wuphf>(_ipcQueue, 2048)),
          _library(std::make_unique<LinuxDynamicLibrary>(
              _config.software.firmwareFile))
    {
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
        _ipcThread =
            std::make_unique<std::thread>([this]() { _ipcClient->run(); });

        if (!_ipcClient->waitForConnection())
        {
            _ipcClient->stop();
            _ipcThread->join();
            throw IpcException("Error connecting to IPC server");
        }

        _ipcThread->detach();
    }

    void Runner::_configureIpc()
    {
        _ipcClient = std::visit(IpcModeVisitor(_ipcProtocol), _config.ipc);
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

        sabre::impl::pilot::Factory fac(nullptr);
        sabre::core::ResourceManager rm(fac, config);

        _entryPointFn(rm);
    }

    void Runner::start()
    {
        _loadEntryPoint();
        _configureIpc();
        _startIpc();
        _startFirmware();
    }
} // namespace sabre_runner::core