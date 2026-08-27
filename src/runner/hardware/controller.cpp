#include "controller.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace sabre_runner::hardware
{
    Controller::Controller(sabre_runner::core::HardwareConfig config,
                           SendCommandHandler sendCommandHandler)
        : _config(config), _sendCommandHandler(std::move(sendCommandHandler)),
          _uartControllers(_config.upperboundUart)
    {
        for (size_t uartIdx = 0; uartIdx < _uartControllers.size(); uartIdx++)
        {
            _uartControllers[uartIdx].setOutputBufferCallback(
                [this, uartIdx](char c) { _uartFlushCallback(uartIdx, c); });
        }
    }

    void Controller::_uartFlushCallback(size_t uartIndex, char byte)
    {
        std::cout << uartIndex << " --> " << byte << '\n' << std::flush;
    }

    UartController &Controller::getUartController(size_t uartNumber)
    {
        if (uartNumber >= _config.upperboundUart)
            throw DeviceUartNotConfiguredException("Uart number too big");
        return _uartControllers[uartNumber];
    }
} // namespace sabre_runner::hardware