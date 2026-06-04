#pragma once

#include <functional>
#include <sabre/core/resource_manager.hpp>
#include <string>
#include <unordered_map>

namespace sabre_pilot
{
    using LibraryEntryPointRaw = int (*)(sabre::core::ResourceManager &);
    using LibraryEntryPoint =
        std::function<int(sabre::core::ResourceManager &)>;

    class DynamicLibrary
    {
    private:
        const std::string _libLocation;

    protected:
        std::unordered_map<std::string, LibraryEntryPoint> _entryPoints;

        const std::string &_getLibLocation() const;

    public:
        DynamicLibrary(const std::string &libLocation);
        virtual ~DynamicLibrary() = default;
        virtual void load() = 0;

        void addEntryPoint(const std::string &name);
        LibraryEntryPoint &getEntryPoint(const std::string &name);
    };
} // namespace sabre_pilot