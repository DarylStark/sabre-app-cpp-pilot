#pragma once

#include <functional>
#include <sabre/core/resource_manager.hpp>
#include <string>
#include <unordered_map>

namespace sabre_pilot_runner_core
{
    using LibraryEntryPointRaw = int (*)(sabre::core::ResourceManager &);
    using LibraryEntryPoint =
        std::function<int(sabre::core::ResourceManager &)>;

    class DynamicLibrary
    {
    public:
        using UniquePtr = std::unique_ptr<DynamicLibrary>;

    private:
        const std::string _libLocation;

    protected:
        std::unordered_map<std::string, LibraryEntryPoint> _entryPoints;

        const std::string &_getLibLocation() const;
        virtual void _loadEntryPoint(const std::string &entryPoint) = 0;
        virtual void _loadLibrary() = 0;

    public:
        DynamicLibrary(const std::string &libLocation);
        virtual ~DynamicLibrary() = default;

        LibraryEntryPoint &getEntryPoint(const std::string &name);
    };
} // namespace sabre_pilot_runner_core