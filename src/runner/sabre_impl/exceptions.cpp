#include "exceptions.hpp"

namespace sabre::impl::pilot
{
    SabreImplPilotException::SabreImplPilotException(const char *message)
        : std::runtime_error(message)
    {
    }
} // namespace sabre::impl::pilot