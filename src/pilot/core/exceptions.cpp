#include "exceptions.hpp"

namespace sabre_pilot::core
{
    SabrePilotException::SabrePilotException(const char *message)
        : std::runtime_error(message)
    {
    }
} // namespace sabre_pilot::core