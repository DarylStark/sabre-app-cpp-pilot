#include "dynamic_library.hpp"
#include "exceptions.hpp"

namespace sabre_runner::core
{
    DynamicLibrary::DynamicLibrary(const std::string &libLocation)
        : _libLocation(libLocation)
    {
    }

    const std::string &DynamicLibrary::_getLibLocation() const
    {
        return _libLocation;
    }

    LibraryEntryPoint &DynamicLibrary::getEntryPoint(const std::string &name)
    {
        if (_entryPoints.find(name) == _entryPoints.end())
            _loadEntryPoint(name);
        return _entryPoints.at(name);
    }
} // namespace sabre_runner::core