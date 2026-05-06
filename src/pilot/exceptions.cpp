#include "exceptions.hpp"

namespace sabre_pilot::exceptions
{
    SabrePilotException::SabrePilotException(const char *message)
        : std::runtime_error(message)
    {
    }

    DeviceUartNotConfiguredException::DeviceUartNotConfiguredException(
        const char *message)
        : SabrePilotException(message)
    {
    }
} // namespace sabre_pilot::exceptions