#pragma once

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

    class Device
    {
    private:
        std::unique_ptr<std::string[]> _uartBuffers;
        DeviceConfig _config;

    public:
        Device(DeviceConfig config);
        void start();
        void stop();

        const std::string &getUartBuffer(sabre::hal::UartNumber uartIdx) const;
        void clearUartBuffer(sabre::hal::UartNumber uartIdx);
        sabre::hal::UartNumber getUartCount() const;
    };
} // namespace sabre_pilot