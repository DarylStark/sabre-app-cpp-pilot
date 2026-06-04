#pragma once

#include <stdexcept>

namespace sabre_pilot
{
    class SabrePilotException : public std::runtime_error
    {
    public:
        SabrePilotException(const char *message);
    };

    class NonExistingEntryPointException : public SabrePilotException
    {
    public:
        NonExistingEntryPointException(const char *message);
    };

    class DynamicLibraryLoadingException : public SabrePilotException
    {
    public:
        DynamicLibraryLoadingException(const char *message);
    };

    class EntryPointNotInLibraryException : public SabrePilotException
    {
    public:
        EntryPointNotInLibraryException(const char *message);
    };
}; // namespace sabre_pilot