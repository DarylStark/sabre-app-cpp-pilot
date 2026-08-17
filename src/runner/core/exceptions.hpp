#pragma once

#include <stdexcept>

namespace sabre_runner::core
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
}; // namespace sabre_runner::core