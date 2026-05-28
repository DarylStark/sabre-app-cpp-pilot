#pragma once
#include "uart_controller.hpp"
#include <memory>
#include <sabre/core/resource_manager.hpp>
#include <string>

namespace sabre::impl::pilot
{
    class Device
    {
    private:
        sabre::core::ResourceManagerConfig _config;
        std::unique_ptr<UartController[]> _uartOuput = nullptr;

    public:
        using Ptr = Device *;
        using SharedPtr = std::shared_ptr<Device>;
        using UniquePtr = std::unique_ptr<Device>;

        Device(sabre::core::ResourceManagerConfig config);

        UartController &getUartController(size_t uartNumber);
        const sabre::core::ResourceManagerConfig &getConfig() const;
    };
} // namespace sabre::impl::pilot