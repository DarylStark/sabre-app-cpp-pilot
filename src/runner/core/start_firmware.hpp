#pragma once

#include <sabre/core/resource_manager.hpp>

namespace sabre_pilot_runner_core
{
    void startFirmware(sabre::core::ResourceManagerConfig config,
                       const std::string &firmwareFile,
                       const std::string &entryPoint);
}