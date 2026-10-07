#pragma once

#include "config.hpp"
#include "dynamic_library.hpp"
#include "exceptions.hpp"
#include <condition_variable>
#include <cstdint>
#include <hardware/controller.hpp>
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <ipc/queue.hpp>
#include <logger_factory/logger_factory.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <variant>
#include <wuphf/wuphf.hpp>
#include <wuphf/wuphf_message.hpp>

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

        // Logging
        sabre_logger_factory::LoggerFactory &_loggerFactory;
        std::shared_ptr<spdlog::logger> _logger;

        // Software
        DynamicLibrary::UniquePtr _library{};
        LibraryEntryPoint _entryPointFn;

        // Hardware
        sabre_runner::hardware::Controller::SharedPtr _hardware;

        // IPC
        ipc::Queue<sabre::ipc::WuphfMessage::UniquePtr> _ipcQueue;
        ipc::IpcProtocol::SharedPtr _ipcProtocol;
        ipc::IpcClient::SharedPtr _ipcClient{};
        std::unique_ptr<std::thread> _ipcClientThread{};
        std::unique_ptr<std::thread> _ipcReceiverThread{};

        // IPC ready flag
        std::condition_variable _ipcReadyCv;
        std::mutex _ipcReadyMutex;
        bool _ipcReady{false};

        // Starting the application
        void _loadEntryPoint();
        void _configureIpc();
        void _startIpc();
        void _waitForIpcDone();
        void _configureHardware();
        void _startFirmware();

        // Threads
        void _ipcReceiverThreadFn();

    public:
        Runner(CoreConfig config,
               sabre_logger_factory::LoggerFactory &loggerFactory);

        void start();
        void markIpcReady();
    };
} // namespace sabre_runner::core