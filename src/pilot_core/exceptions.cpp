#include "exceptions.hpp"

namespace sabre_pilot
{
    SabrePilotException::SabrePilotException(const char *message)
        : std::runtime_error(message)
    {
    }

    DynamicLibraryLoadingException::DynamicLibraryLoadingException(
        const char *message)
        : SabrePilotException(message)
    {
    }

    EntryPointNotInLibraryException::EntryPointNotInLibraryException(
        const char *message)
        : SabrePilotException(message)
    {
    }
} // namespace sabre_pilot