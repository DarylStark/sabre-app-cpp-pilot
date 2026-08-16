#include "exceptions.hpp"

namespace sabre_pilot_runner_core
{
    SabrePilotRunnerException::SabrePilotRunnerException(const char *message)
        : std::runtime_error(message)
    {
    }

    DynamicLibraryLoadingException::DynamicLibraryLoadingException(
        const char *message)
        : SabrePilotRunnerException(message)
    {
    }

    EntryPointNotInLibraryException::EntryPointNotInLibraryException(
        const char *message)
        : SabrePilotRunnerException(message)
    {
    }
} // namespace sabre_pilot_runner_core