#pragma once

#include <stdexcept>

namespace sabre_runner::hardware
{
    class SabreRunnerHardwareException : public std::runtime_error
    {
    public:
        SabreRunnerHardwareException(const char *message);
    };

    class UartControllerNotInitializedException
        : public SabreRunnerHardwareException
    {
    public:
        UartControllerNotInitializedException(const char *message);
    };

    class DeviceUartNotConfiguredException : public SabreRunnerHardwareException
    {
    public:
        DeviceUartNotConfiguredException(const char *message);
    };
}; // namespace sabre_runner::hardware