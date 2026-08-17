#pragma once

#include <stdexcept>

namespace sabre_pilot::core
{
    class SabrePilotException : public std::runtime_error
    {
    public:
        SabrePilotException(const char *message);
    };
}; // namespace sabre_pilot::core