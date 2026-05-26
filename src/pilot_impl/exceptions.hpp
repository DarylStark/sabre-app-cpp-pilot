#pragma once

#include <stdexcept>

namespace sabre::impl::pilot
{
    class SabreImplPilotException : public std::runtime_error
    {
    public:
        SabreImplPilotException(const char *message);
    };

    class DeviceNotConfiguredException : public SabreImplPilotException
    {
    public:
        DeviceNotConfiguredException(const char *message);
    };

    class InvalidUartIndex : public SabreImplPilotException
    {
    public:
        InvalidUartIndex(const char *message);
    };
} // namespace sabre::impl::pilot