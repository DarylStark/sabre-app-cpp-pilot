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

    // InvalidUartIndex

    InvalidUartIndex::InvalidUartIndex(const char *message)
        : SabreImplPilotException(message)
    {
    }
} // namespace sabre::impl::pilot