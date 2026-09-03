#include "controller.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <thread>
#include <wuphf/wuphf.hpp>
#include <wuphf/wuphf_message.hpp>

namespace sabre_runner::hardware
{
    Controller::Controller(sabre_runner::core::HardwareConfig config,
                           ipc::IpcClient::SharedPtr ipcClient)
        : _config(config), _ipcClient(std::move(ipcClient)),
          _uartControllers(_config.upperboundUart)
    {
        for (size_t uartIdx = 0; uartIdx < _uartControllers.size(); uartIdx++)
        {
            _uartControllers[uartIdx].setOutputBufferCallback(
                [this, uartIdx](const std::string &data)
                { _uartFlushCallback(uartIdx, data); });
        }
    }

    void Controller::_uartFlushCallback(size_t uartIndex,
                                        const std::string &data)
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << uartIndex << " --> " << data << '\n' << std::flush;
        sabre::ipc::UartAppend append(0, uartIndex, data);
        sabre::ipc::sendWuphfMessage(*_ipcClient, append);
    }

    UartController &Controller::getUartController(size_t uartNumber)
    {
        if (uartNumber >= _config.upperboundUart)
            throw DeviceUartNotConfiguredException("Uart number too big");
        return _uartControllers[uartNumber];
    }
} // namespace sabre_runner::hardware