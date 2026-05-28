#include "core.hpp"
#include "hal.hpp"
#include <iostream>

namespace sabre::impl::pilot
{
    Factory::Factory(sabre_pilot::Device *device) : _device(device) {}

    Factory::~Factory() {}

    sabre::hal::Serial::UniquePtr Factory::createUartObject(
        sabre::hal::UartNumber uartNumber, sabre::hal::BaudRate baudRate,
        sabre::hal::PinNumber txPin, sabre::hal::PinNumber rxPin,
        size_t bufferSize) const
    {
        return std::make_unique<sabre::impl::pilot::Uart>(_device, uartNumber);
    }

    sabre::hal::Serial::UniquePtr Factory::createUsbCdc(uint32_t index,
                                                        size_t bufferSize) const
    {
        return nullptr;
    }

    sabre::hal::InputGpio::UniquePtr
    Factory::createInputGpio(sabre::hal::PinNumber pin) const
    {
        return nullptr;
    }

    sabre::hal::OutputGpio::UniquePtr
    Factory::createOutputGpio(sabre::hal::PinNumber pin) const
    {
        return nullptr;
    }

    sabre::hal::Gpio::UniquePtr
    Factory::createGpio(sabre::hal::PinNumber pin) const
    {
        return std::make_unique<sabre::impl::pilot::Gpio>(_device, pin);
    }

    sabre::net::WifiStation::UniquePtr Factory::createWifiStation() const
    {
        return nullptr;
    }

    sabre::net::WifiSoftAp::UniquePtr Factory::createWifiSoftAp() const
    {
        return nullptr;
    }

    sabre::time::WallClock::UniquePtr Factory::createWallClock() const
    {
        return nullptr;
    }

    sabre::time::NtpClient::UniquePtr
    Factory::createNtpClient(const std::string &server) const
    {
        return nullptr;
    }

    sabre::net::MqttClient::UniquePtr Factory::createMqttClient() const
    {
        return nullptr;
    }

    sabre::utility::WaitFor::UniquePtr
    Factory::createWaitFor(sabre::utility::WaitForPred fn, uint64_t timeoutInMs,
                           uint64_t sleepTime) const
    {
        return nullptr;
    }

    sabre::os::Service::UniquePtr
    Factory::createService(sabre::os::ServiceHandler fn) const
    {
        return nullptr;
    }

    sabre::platform::Platform::UniquePtr Factory::createPlatform() const
    {
        return nullptr;
    }

    sabre::os::Queue::UniquePtr
    Factory::createQueue(std::size_t capacity, std::size_t elementSize) const
    {
        return nullptr;
    }

    sabre::net::HttpServer::UniquePtr Factory::createHttpServer() const
    {
        return nullptr;
    }

    sabre::devices::RgbPixelStrip::UniquePtr
    Factory::createRgbPixelStrip(sabre::hal::PinNumber pinNumber,
                                 sabre::devices::PixelIndex length) const
    {
        return nullptr;
    }

} // namespace sabre::impl::pilot