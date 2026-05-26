#pragma once
#include "uart_controller.hpp"
#include <memory>
#include <sabre/core/resource_manager.hpp>
#include <string>

namespace sabre_pilot
{
    class Device
    {
    private:
        sabre::core::ResourceManagerConfig _config;
        std::unique_ptr<UartController[]> _uartOuput = nullptr;

    public:
        Device(sabre::core::ResourceManagerConfig config);

        UartController &outputUart(size_t uartNumber);
        const sabre::core::ResourceManagerConfig &config() const;
    };
} // namespace sabre_pilot