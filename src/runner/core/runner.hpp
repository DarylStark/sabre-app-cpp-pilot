#pragma once

#include "config.hpp"
#include "dynamic_library.hpp"
#include "exceptions.hpp"
#include <cstdint>
#include <hardware/controller.hpp>
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

        // Software
        DynamicLibrary::UniquePtr _library{};
        LibraryEntryPoint _entryPointFn;

        // Hardware
        sabre_runner::hardware::Controller::SharedPtr _hardware;

        // IPC
        ipc::Queue<sabre::ipc::WuphfCommand::UniquePtr> _ipcQueue;
        ipc::IpcProtocol::SharedPtr _ipcProtocol;
        ipc::IpcClient::SharedPtr _ipcClient{};
        std::unique_ptr<std::thread> _ipcThread{};

        void _loadEntryPoint();
        void _configureIpc();
        void _configureHardware();
        void _startIpc();
        void _startFirmware();

    public:
        Runner(CoreConfig config);

        void start();
    };
} // namespace sabre_runner::core