#include "dynamic_library.hpp"
#include "exceptions.hpp"

namespace sabre_pilot
{
    DynamicLibrary::DynamicLibrary(const std::string &libLocation)
        : _libLocation(libLocation)
    {
    }

    const std::string &DynamicLibrary::_getLibLocation() const
    {
        return _libLocation;
    }

    void DynamicLibrary::addEntryPoint(const std::string &name)
    {
        _entryPoints[name] = nullptr;
    }

    LibraryEntryPoint &DynamicLibrary::getEntryPoint(const std::string &name)
    {
        if (_entryPoints.find(name) == _entryPoints.end())
            throw NonExistingEntryPointException("Entrypoint doesn't exist");
        return _entryPoints.at(name);
    }
} // namespace sabre_pilot