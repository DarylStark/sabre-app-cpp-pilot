#include "core.hpp"
#include "hal.hpp"
#include <iostream>

namespace sabre::impl::pilot
{
    Factory::Factory(Device *device) : _device(device)
    {
        std::cout << "Factory created for device at " << device << '\n'
                  << std::flush;
    }

    sabre::hal::Serial::UniquePtr Factory::createUartObject(
        sabre::hal::UartNumber uartNumber, sabre::hal::BaudRate baudRate,
        sabre::hal::PinNumber txPin, sabre::hal::PinNumber rxPin,
        size_t bufferSize) const
    {
        // Implementation specific code to create a UART object
        return std::make_unique<Serial>(_device, uartNumber, bufferSize);
    }

    sabre::hal::Serial::UniquePtr Factory::createUsbCdc(uint32_t index,
                                                        size_t bufferSize) const
    {
        // Implementation specific code to create a USB CDC object
        return nullptr; // Placeholder
    }

    sabre::hal::InputGpio::UniquePtr
    Factory::createInputGpio(sabre::hal::PinNumber pin) const
    {
        std::cout << "Creating InputGPIO on pin " << pin << " for device at "
                  << _device << '\n'
                  << std::flush;
        return std::make_unique<sabre::impl::pilot::InputGpio>(_device, pin);
    }

    sabre::hal::OutputGpio::UniquePtr
    Factory::createOutputGpio(sabre::hal::PinNumber pin) const
    {
        std::cout << "Creating OutputGPIO on pin " << pin << " for device at "
                  << _device << '\n'
                  << std::flush;
        return std::make_unique<sabre::impl::pilot::OutputGpio>(_device, pin);
    }

    sabre::hal::Gpio::UniquePtr
    Factory::createGpio(sabre::hal::PinNumber pin) const
    {
        std::cout << "Creating generic GPIO on pin " << pin << " for device at "
                  << _device << '\n'
                  << std::flush;
        return std::make_unique<sabre::impl::pilot::Gpio>(_device, pin);
    }

    sabre::net::WifiStation::UniquePtr Factory::createWifiStation() const
    {
        // Implementation specific code to create a WifiStation object
        return nullptr; // Placeholder
    }

    sabre::net::WifiSoftAp::UniquePtr Factory::createWifiSoftAp() const
    {
        // Implementation specific code to create a WifiSoftAP object
        return nullptr; // Placeholder
    }

    sabre::time::WallClock::UniquePtr Factory::createWallClock() const
    {
        // Implementation specific code to create a WallClock object
        return nullptr; // Placeholder
    }

    sabre::time::NtpClient::UniquePtr
    Factory::createNtpClient(const std::string &server) const
    {
        // Implementation specific code to create an NTPClient object
        return nullptr; // Placeholder
    }

    sabre::net::MqttClient::UniquePtr Factory::createMqttClient() const
    {
        // Implementation specific code to create an MQTTClient object
        return nullptr; // Placeholder
    }

    sabre::utility::WaitFor::UniquePtr
    Factory::createWaitFor(sabre::utility::WaitForPred fn, uint64_t timeoutInMs,
                           uint64_t sleepTime) const
    {
        // Implementation specific code to create a WaitFor object
        return nullptr; // Placeholder
    }

    sabre::os::Service::UniquePtr
    Factory::createService(sabre::os::ServiceHandler fn) const
    {
        // Implementation specific code to create a Service object
        return nullptr; // Placeholder
    }
} // namespace sabre::impl::pilot