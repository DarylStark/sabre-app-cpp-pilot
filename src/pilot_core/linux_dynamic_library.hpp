#pragma once

#include "dynamic_library.hpp"
#include <string>

namespace sabre_pilot
{
    class LinuxDynamicLibrary : public DynamicLibrary
    {
    private:
        void *_libHandle = nullptr;
        void _loadEntryPoint(const std::string &entryPoint) override;
        void _loadLibrary() override;
        void _closeLibrary();

    public:
        LinuxDynamicLibrary(const std::string &libLocation);
        ~LinuxDynamicLibrary();
    };
} // namespace sabre_pilot