#pragma once

#include "exceptions.hpp"
#include <cstdint>
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <ipc/queue.hpp>
#include <memory>
#include <string>
#include <thread>
#include <variant>
#include <wuphf/wuphf_command.hpp>

namespace sabre_runner::core
{
    struct HardwareConfig
    {
        uint32_t maxGpios = 1;
        uint32_t upperboundUart = 1;
    };

    struct SoftwareConfig
    {
        std::string firmwareFile{};
        std::string entryPoint{};
    };

    enum class IpcMode
    {
        None,
        Tcp
    };

    struct IpcNoneConfig
    {
    };

    struct IpcTcpConfig
    {
        std::string serverIp;
        uint16_t serverPort;
    };

    struct CoreConfig
    {
        uint32_t deviceId;
        HardwareConfig hardware;
        SoftwareConfig software;
        std::variant<IpcNoneConfig, IpcTcpConfig> ipc;
    };

    class IpcModeVisitor
    {
    private:
        ipc::IpcProtocol::SharedPtr _ipcProtocol;

    public:
        IpcModeVisitor(ipc::IpcProtocol::SharedPtr protocol);
        ipc::IpcClient::SharedPtr operator()(IpcNoneConfig &config);
        ipc::IpcClient::SharedPtr operator()(IpcTcpConfig &config);

        template <typename T>
        ipc::IpcClient::SharedPtr operator()(const T &value) const
        {
            throw UnknownIpcMode("Unknown IPC mode.");
        }
    };

    class Runner
    {
    private:
        CoreConfig _config;

        // IPC
        ipc::Queue<sabre::ipc::WuphfCommand::UniquePtr> _ipcQueue;
        ipc::IpcProtocol::SharedPtr _ipcProtocol;
        ipc::IpcClient::SharedPtr _ipcClient{};
        std::unique_ptr<std::thread> _ipcThread{};

        void _configureIpc();
        void _startIpc();

    public:
        Runner(CoreConfig config);

        void start();
    };
} // namespace sabre_runner::core