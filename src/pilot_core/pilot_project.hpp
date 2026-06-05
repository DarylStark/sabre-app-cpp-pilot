#pragma once

#include <sabre/core/resource_manager.hpp>
#include <vector>

namespace sabre_pilot
{
    struct ProjectDevice
    {
        sabre::core::ResourceManagerConfig config;
        std::string name;
        std::string library;
        std::string entryPoint = "startApp";
    };

    struct Project
    {
        std::vector<ProjectDevice> devices;
    };
} // namespace sabre_pilot