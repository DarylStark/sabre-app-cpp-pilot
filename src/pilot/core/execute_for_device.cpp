#include "execute_for_device.hpp"
#include <iostream>

namespace sabre_pilot::core
{
    void ExecuteForDevice::setDevice(std::shared_ptr<Device> device)
    {
        _device = std::move(device);
    }

    void ExecuteForDevice::visitClientHello(sabre::ipc::ClientHello &message)
    {
        std::cout << "Hello from visitor for " << message.getDestinationMcuId()
                  << '\n';
    }

    void ExecuteForDevice::visitUartAppend(sabre::ipc::UartAppend &message)
    {
        // TODO: Implement for real; before doing that, the `UartAppend` has
        // to get methods to retrieve the data (uart index and text).
        std::cout << "HERE\n";
        _device->appendToUArt(message.getUartIdx(), message.getData());
    }
} // namespace sabre_pilot::core