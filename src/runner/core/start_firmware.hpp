#pragma once

#include <sabre/core/resource_manager.hpp>

namespace sabre_runner::core
{
    void startFirmware(sabre::core::ResourceManagerConfig config,
                       const std::string &firmwareFile,
                       const std::string &entryPoint);
}