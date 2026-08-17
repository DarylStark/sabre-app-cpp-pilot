#include "device.hpp"
#include <iostream>

namespace sabre_pilot::core
{
    Device::Device(uint32_t id, DeviceConfig config,
                   const SubprocessStrategy &subprocessStrategy,
                   const std::string &runnerExec)
        : _id(id), _config(std::move(config)),
          _subprocessStrategy(subprocessStrategy), _runnerExec(runnerExec)
    {
        _uartBuffers = std::make_unique<std::string[]>(
            _config.deviceConfig.upperboundUart);
    }

    Device::~Device()
    {
        stop();
    }

    void Device::start()
    {
        _firmwarePid = _subprocessStrategy.start(
            _runnerExec,
            {_config.firmwarePath, std::to_string(_id),
             "--firmware-entry-point", _config.firmwareEntryPoint});
        if (!_firmwarePid)
        {
            _state = DeviceState::Error;
        }
        else
        {
            _state = DeviceState::Starting;
        }
    }

    void Device::stop()
    {
        if (_firmwarePid == 0)
            return;
        _state = DeviceState::Stopping;
        _subprocessStrategy.stop(_firmwarePid);
    }

    uint32_t Device::getId() const
    {
        return _id;
    }

    const std::string &
    Device::getUartBuffer(sabre::hal::UartNumber uartIdx) const
    {
        return _uartBuffers[uartIdx];
    }

    void Device::clearUartBuffer(sabre::hal::UartNumber uartIdx)
    {
        _uartBuffers[uartIdx].clear();
    }

    sabre::hal::UartNumber Device::getUartCount() const
    {
        return _config.deviceConfig.upperboundUart;
    }

    void Device::appendToUArt(sabre::hal::UartNumber uartIdx,
                              const std::string &data)
    {
        std::string &buffer = _uartBuffers[uartIdx];
        buffer.append(data);
    }

    const DeviceState Device::getState() const
    {
        return _state;
    }

    const uint32_t Device::getPid() const
    {
        return _firmwarePid;
    }

    void Device::updateState()
    {
        if (_firmwarePid == 0)
            return;
        if (_subprocessStrategy.isRunning(_firmwarePid))
        {
            _state = DeviceState::Running;
        }
        else
        {
            _firmwarePid = 0;
            _state = DeviceState::Stopped;
        }
    }
} // namespace sabre_pilot::core