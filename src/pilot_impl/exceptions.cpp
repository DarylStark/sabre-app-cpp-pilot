#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    SabreImplPilotException::SabreImplPilotException(const char *message)
        : std::runtime_error(message)
    {
    }

    DeviceNotConfiguredException::DeviceNotConfiguredException(
        const char *message)
        : SabreImplPilotException(message)
    {
    }

    InvalidUartIndex::InvalidUartIndex(const char *message)
        : SabreImplPilotException(message)
    {
    }

    SabrePilotException::SabrePilotException(const char *message)
        : SabreImplPilotException(message)
    {
    }

    DeviceUartNotConfiguredException::DeviceUartNotConfiguredException(
        const char *message)
        : SabrePilotException(message)
    {
    }

    UartControllerNotInitializedException::
        UartControllerNotInitializedException(const char *message)
        : SabrePilotException(message)
    {
    }
} // namespace sabre::impl::pilot