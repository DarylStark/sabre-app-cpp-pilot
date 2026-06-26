#include "device.hpp"
#include <iostream>

namespace sabre_pilot
{
    Device::Device(DeviceConfig config,
                   const SubprocessStrategy &subprocessStrategy,
                   const std::string &runnerExec)
        : _config(std::move(config)), _subprocessStrategy(subprocessStrategy),
          _runnerExec(runnerExec)
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
            _runnerExec, {_config.firmwarePath, _config.firmwareEntryPoint});
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
} // namespace sabre_pilot