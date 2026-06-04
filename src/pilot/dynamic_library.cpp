#include "dynamic_library.hpp"

namespace sabre_pilot
{
    DynamicLibrary::DynamicLibrary(const std::string &libLocation)
        : _libLocation(libLocation)
    {
        _entryPoints["startApp"] = nullptr;
    }

    const std::string &DynamicLibrary::_getLibLocation() const
    {
        return _libLocation;
    }

    LibraryEntryPoint &DynamicLibrary::getEntryPoint(const std::string &name)
    {
        return _entryPoints.at(name);
    }
} // namespace sabre_pilot