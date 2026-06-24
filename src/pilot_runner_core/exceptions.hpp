#pragma once

#include <stdexcept>

namespace sabre_pilot_runner_core
{
    class SabrePilotRunnerException : public std::runtime_error
    {
    public:
        SabrePilotRunnerException(const char *message);
    };

    class DynamicLibraryLoadingException : public SabrePilotRunnerException
    {
    public:
        DynamicLibraryLoadingException(const char *message);
    };

    class EntryPointNotInLibraryException : public SabrePilotRunnerException
    {
    public:
        EntryPointNotInLibraryException(const char *message);
    };
}; // namespace sabre_pilot_runner_core