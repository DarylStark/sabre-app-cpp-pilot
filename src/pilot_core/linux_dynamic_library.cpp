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
        _closeLibrary();
    }

    void LinuxDynamicLibrary::_loadEntryPoint(const std::string &entryPoint)
    {
        _loadLibrary();
        auto ep = reinterpret_cast<LibraryEntryPointRaw>(
            dlsym(_libHandle, entryPoint.c_str()));
        auto entryPointHandle = static_cast<LibraryEntryPoint>(ep);

        if (!entryPointHandle)
            throw EntryPointNotInLibraryException("Entry point not in library");
        _entryPoints[entryPoint] = entryPointHandle;
    }

    void LinuxDynamicLibrary::_loadLibrary()
    {
        if (_libHandle != nullptr)
            return;
        _libHandle = dlopen(_getLibLocation().c_str(), RTLD_NOW);
        if (!_libHandle)
            throw DynamicLibraryLoadingException(
                "Dynamic library could not be loaded");
    }

    void LinuxDynamicLibrary::_closeLibrary()
    {
        if (_libHandle)
            dlclose(_libHandle);
    }
} // namespace sabre_pilot