#pragma once

#include <stdexcept>

namespace sabre_pilot::exceptions
{
    class SabrePilotException : public std::runtime_error
    {
    public:
        SabrePilotException(const char *message);
    };

    class DeviceUartNotConfiguredException : public SabrePilotException
    {
    public:
        DeviceUartNotConfiguredException(const char *message);
    };
} // namespace sabre_pilot::exceptions