#include "start_firmware.hpp"
#include "linux_dynamic_library.hpp"
#include <sabre/core/resource_manager.hpp>
#include <sabre_impl/core.hpp>

namespace sabre_pilot_runner_core
{
    void startFirmware(sabre::core::ResourceManagerConfig config,
                       const std::string &firmwareFile,
                       const std::string &entryPoint)
    {
        sabre::impl::pilot::Factory fac(nullptr);
        sabre::core::ResourceManager rm(fac, config);

        sabre_pilot_runner_core::DynamicLibrary::UniquePtr firmware =
            std::make_unique<sabre_pilot_runner_core::LinuxDynamicLibrary>(
                firmwareFile);
        firmware->getEntryPoint(entryPoint)(rm);
    }
} // namespace sabre_pilot_runner_core