#include "exceptions.hpp"

namespace sabre_runner::hardware
{
    SabreRunnerHardwareException::SabreRunnerHardwareException(
        const char *message)
        : std::runtime_error(message)
    {
    }

    UartControllerNotInitializedException::
        UartControllerNotInitializedException(const char *message)
        : SabreRunnerHardwareException(message)
    {
    }

    DeviceUartNotConfiguredException::DeviceUartNotConfiguredException(
        const char *message)
        : SabreRunnerHardwareException(message)
    {
    }
} // namespace sabre_runner::hardware