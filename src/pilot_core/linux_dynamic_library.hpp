#pragma once

#include "dynamic_library.hpp"
#include <string>

namespace sabre_pilot
{
    class LinuxDynamicLibrary : public DynamicLibrary
    {
    private:
        void *_libHandle = nullptr;

    public:
        LinuxDynamicLibrary(const std::string &libLocation);
        ~LinuxDynamicLibrary();

        void load() override;
    };
} // namespace sabre_pilot