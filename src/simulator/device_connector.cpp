#include "device_connector.hpp"

namespace sabre::impl::pilot
{
    UartConnector::UartConnector(Device &deviceA, uint32_t uartNumberA,
                                 Device &deviceB, uint32_t uartNumberB)
        : _deviceA(deviceA), _uartNumberA(uartNumberA), _deviceB(deviceB),
          _uartNumberB(uartNumberB)
    {
        _deviceA.register_event_callback(
            DeviceEventType::UART_DATA_SEND,
            [this](const DeviceEvent &event)
            {
                auto *data = dynamic_cast<UartEventData *>(event.data);
                if (data && data->uartNumber == _uartNumberA)
                    _deviceB.add_to_input_uart_buffer(
                        _uartNumberB, std::string(1, data->data));
            });
    }
} // namespace sabre::impl::pilot