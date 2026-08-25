#include "runner.hpp"
#include "exceptions.hpp"
#include <ipc_tcp/client.hpp>
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
          _ipcProtocol(std::make_shared<sabre::ipc::Wuphf>(_ipcQueue, 2048))
    {
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

    void Runner::start()
    {
        _configureIpc();
        _startIpc();
    }
} // namespace sabre_runner::core