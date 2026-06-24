#pragma once

#include <stdexcept>

namespace sabre_pilot
{
    class SabrePilotException : public std::runtime_error
    {
    public:
        SabrePilotException(const char *message);
    };
}; // namespace sabre_pilot