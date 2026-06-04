#include "linux_dynamic_library.hpp"
#include <dlfcn.h>
#include <iostream>

namespace sabre_pilot
{
    LinuxDynamicLibrary::LinuxDynamicLibrary(const std::string &libLocation)
        : DynamicLibrary(libLocation)
    {
    }

    LinuxDynamicLibrary::~LinuxDynamicLibrary()
    {
        if (_libHandle)
            dlclose(_libHandle);
    }

    void LinuxDynamicLibrary::load()
    {
        if (_libHandle)
            return;
        _libHandle = dlopen(_getLibLocation().c_str(), RTLD_NOW);

        // TODO: Exception when it went wrong

        for (auto &[name, entry_point] : _entryPoints)
        {
            if (!entry_point)
            {
                auto ep = reinterpret_cast<LibraryEntryPointRaw>(
                    dlsym(_libHandle, name.c_str()));
                entry_point = static_cast<LibraryEntryPoint>(ep);

                // TODO: Exception when it went wrong
            }
        }
    }
} // namespace sabre_pilot