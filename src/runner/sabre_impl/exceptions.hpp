#pragma once

#include <stdexcept>

namespace sabre::impl::pilot
{
    class SabreImplPilotException : public std::runtime_error
    {
    public:
        SabreImplPilotException(const char *message);
    };

} // namespace sabre::impl::pilot