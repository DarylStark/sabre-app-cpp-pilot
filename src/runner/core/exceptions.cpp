#include "exceptions.hpp"

namespace sabre_runner::core
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

    UnknownIpcMode::UnknownIpcMode(const char *message)
        : SabrePilotRunnerException(message)
    {
    }

    IpcException::IpcException(const char *message)
        : SabrePilotRunnerException(message)
    {
    }

    NotConfigureException::NotConfigureException(const char *message)
        : SabrePilotRunnerException(message)
    {
    }
} // namespace sabre_runner::core