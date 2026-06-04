#include "linux_dynamic_library.hpp"
#include "exceptions.hpp"
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
        if (!_libHandle)
        {
            _libHandle = dlopen(_getLibLocation().c_str(), RTLD_NOW);
            if (!_libHandle)
                throw DynamicLibraryLoadingException(
                    "Dynamic library could not be loaded");
        }

        for (auto &[name, entry_point] : _entryPoints)
        {
            if (!entry_point)
            {
                auto ep = reinterpret_cast<LibraryEntryPointRaw>(
                    dlsym(_libHandle, name.c_str()));
                entry_point = static_cast<LibraryEntryPoint>(ep);

                if (!entry_point)
                    throw EntryPointNotInLibraryException(
                        "Entry point not in library");
            }
        }
    }
} // namespace sabre_pilot