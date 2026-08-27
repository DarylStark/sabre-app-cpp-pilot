#pragma once

#include "uart_controller.hpp"
#include <core/config.hpp>
#include <functional>
#include <ipc/client.hpp>
#include <memory>
#include <vector>

namespace sabre_runner::hardware
{
    using SendCommandHandler = std::function<void()>;

    class Controller
    {
    public:
        using SharedPtr = std::shared_ptr<Controller>;
        using UniquePtr = std::unique_ptr<Controller>;
        using Ptr = Controller *;

    private:
        sabre_runner::core::HardwareConfig _config;
        ipc::IpcClient::SharedPtr _ipcClient{};
        std::vector<UartController> _uartControllers;
        SendCommandHandler _sendCommandHandler;

        void _uartFlushCallback(size_t uartIndex, char byte);

    public:
        Controller(sabre_runner::core::HardwareConfig config,
                   SendCommandHandler sendCommandHandler);

        UartController &getUartController(size_t uartNumber);
    };
} // namespace sabre_runner::hardware