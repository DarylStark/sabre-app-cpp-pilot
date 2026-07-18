#pragma once

#include "subprocess_strategy.hpp"
#include <functional>
#include <memory>
#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot
{
    struct DeviceConfig
    {
        sabre::core::ResourceManagerConfig deviceConfig;
        std::string firmwarePath;
        std::string firmwareEntryPoint;
    };

    enum class DeviceState
    {
        Error,
        Running,
        Starting,
        Stopped,
        Stopping
    };

    class Device
    {
    private:
        std::unique_ptr<std::string[]> _uartBuffers;
        DeviceConfig _config;
        const SubprocessStrategy &_subprocessStrategy;
        const std::string &_runnerExec;
        uint32_t _firmwarePid = 0;
        DeviceState _state = DeviceState::Stopped;

    public:
        Device(DeviceConfig config,
               const SubprocessStrategy &subprocessStrategy,
               const std::string &runnerExec);
        ~Device();
        void start();
        void stop();

        const std::string &getUartBuffer(sabre::hal::UartNumber uartIdx) const;
        void clearUartBuffer(sabre::hal::UartNumber uartIdx);
        sabre::hal::UartNumber getUartCount() const;
        void appendToUArt(sabre::hal::UartNumber uartIdx,
                          const std::string &data);
        const DeviceState getState() const;
        const uint32_t getPid() const;

        void updateState();
    };
} // namespace sabre_pilot