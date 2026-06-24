#pragma once

#include "dynamic_library.hpp"
#include <string>

namespace sabre_pilot_runner
{
    class LinuxDynamicLibrary : public DynamicLibrary
    {
    public:
        using UniquePtr = std::unique_ptr<LinuxDynamicLibrary>;

    private:
        void *_libHandle = nullptr;
        void _loadEntryPoint(const std::string &entryPoint) override;
        void _loadLibrary() override;
        void _closeLibrary();

    public:
        LinuxDynamicLibrary(const std::string &libLocation);
        ~LinuxDynamicLibrary();
    };
} // namespace sabre_pilot_runner