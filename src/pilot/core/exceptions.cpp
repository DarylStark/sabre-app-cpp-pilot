#include "exceptions.hpp"

namespace sabre_pilot
{
    SabrePilotException::SabrePilotException(const char *message)
        : std::runtime_error(message)
    {
    }
} // namespace sabre_pilot