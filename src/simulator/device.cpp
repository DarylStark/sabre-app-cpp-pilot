#include "device.hpp"
#include "../sabre_pilot/core.hpp"
#include <iostream>

namespace sabre::impl::pilot
{
    DeviceEventData::~DeviceEventData() {}

    UartEventData::UartEventData(sabre::hal::UartNumber uartNumber, char data)
        : uartNumber(uartNumber), data(data)
    {
    }

    Device::Device(DeviceConfig config, sabre::runtime::App::UniquePtr &&app)
        : _config(config), _app(std::move(app)), _gpios(config.gpioCount)
    {
        uint32_t gpio_index = 0;
        for (auto &gpio : _gpios)
            gpio.number = gpio_index++;
        asm("nop");
    }

    void Device::_raise_event(DeviceEventType type,
                              std::unique_ptr<DeviceEventData> data)
    {
        auto range = _eventCallback.equal_range(type);
        for (auto it = range.first; it != range.second; ++it)
        {
            DeviceEventCallback callback = it->second;
            if (callback)
                callback({type, this, data.get()});
        }
    }

    void Device::start()
    {
        _app->setFactory(std::make_unique<sabre::impl::pilot::Factory>(this));
        _app->start();
    }

    void Device::register_event_callback(DeviceEventType type,
                                         const DeviceEventCallback &callback)
    {
        _eventCallback.emplace(type, callback);
    }

    DeviceGPIO &Device::get_gpio(size_t index)
    {
        if (index >= _gpios.size())
            throw std::out_of_range("GPIO index out of range");
        return _gpios[index];
    }

    void Device::set_gpio_type(size_t index, GPIOType type)
    {
        get_gpio(index).type = type;
    }

    void Device::reset_gpio(size_t index)
    {
        auto &gpio = get_gpio(index);
        gpio.type = GPIOType::GENERIC;
        gpio.state = 0;
    }

    void Device::set_gpio_state(size_t index, uint32_t state)
    {
        get_gpio(index).state = state;
        // TODO: Call ISR if this is a Input GPIO
    }

    GPIOVector Device::get_gpios(GPIOType type) const
    {
        GPIOVector gpios;
        std::copy_if(_gpios.begin(), _gpios.end(), std::back_inserter(gpios),
                     [&type](DeviceGPIO gpio) { return gpio.type == type; });
        return gpios;
    }

    bool Device::initialize_uart(sabre::hal::UartNumber uartNumber,
                                 size_t inputBufferSize)
    {
        if (uartNumber >= _config.uartCount)
            return false;

        if (_uartMap.find(uartNumber) != _uartMap.end())
            return false; // UART already initialized

        _uartMap[uartNumber] =
            UartBuffers{.inputBufferMaxSize = inputBufferSize};
        return true;
    }

    bool Device::deinitialize_uart(sabre::hal::UartNumber uartNumber)
    {
        auto it = _uartMap.find(uartNumber);
        if (it == _uartMap.end())
            return false; // UART not initialized

        _uartMap.erase(it);
        return true;
    }

    bool Device::write_uart_data(sabre::hal::UartNumber uartNumber, char data)
    {
        auto it = _uartMap.find(uartNumber);
        if (it == _uartMap.end())
            return false; // UART not initialized

        it->second.outputData.push_back(data);
        // TODO: only on flush.
        _raise_event(DeviceEventType::UART_DATA_SEND,
                     std::make_unique<UartEventData>(uartNumber, data));

        return true;
    }

    std::string Device::read_uart_data(sabre::hal::UartNumber uartNumber,
                                       size_t maxBytes,
                                       sabre::types::MsTime timeoutInMs)
    {
        auto it = _uartMap.find(uartNumber);
        if (it == _uartMap.end())
            return ""; // UART not initialized

        std::string &input_buffer = it->second.input_buffer;
        std::string result = input_buffer.substr(0, maxBytes);
        it->second.inputDataConsumed += result;
        input_buffer.erase(0, result.size());
        return result;
    }

    void Device::add_to_input_uart_buffer(sabre::hal::UartNumber uartNumber,
                                          const std::string &data)
    {
        auto it = _uartMap.find(uartNumber);
        if (it == _uartMap.end())
            return; // UART not initialized

        // Make sure the input buffer does not exceed max size
        size_t max_size = it->second.inputBufferMaxSize;
        if (it->second.input_buffer.size() + data.size() > max_size)
        {
            size_t excess_size =
                (it->second.input_buffer.size() + data.size()) - max_size;
            std::string to_add = data.substr(excess_size);
            it->second.input_buffer += to_add;
        }
        else
        {
            it->second.input_buffer += data;
        }
    }

    const UARTMap &Device::get_uart_map() const
    {
        return _uartMap;
    }
} // namespace sabre::impl::pilot
