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

    class UnknownIpcMode : public SabrePilotRunnerException
    {
    public:
        UnknownIpcMode(const char *message);
    };

    class IpcException : public SabrePilotRunnerException
    {
    public:
        IpcException(const char *message);
    };

    class NotConfigureException : public SabrePilotRunnerException
    {
    public:
        NotConfigureException(const char *message);
    };
}; // namespace sabre_runner::core