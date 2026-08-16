#pragma once
#include "uart_controller.hpp"
#include <memory>
#include <sabre/core/resource_manager.hpp>
#include <string>

namespace sabre::impl::pilot
{
    class Mcu
    {
    private:
        sabre::core::ResourceManagerConfig _config;
        std::unique_ptr<UartController[]> _uartOuput = nullptr;

    public:
        using Ptr = Mcu *;
        using SharedPtr = std::shared_ptr<Mcu>;
        using UniquePtr = std::unique_ptr<Mcu>;

        Mcu(sabre::core::ResourceManagerConfig config);

        UartController &getUartController(size_t uartNumber);
        const sabre::core::ResourceManagerConfig &getConfig() const;
    };
} // namespace sabre::impl::pilot