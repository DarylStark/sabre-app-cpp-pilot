#include "start_firmware.hpp"
#include "linux_dynamic_library.hpp"
#include <sabre/core/resource_manager.hpp>
#include <sabre_impl/core.hpp>

namespace sabre_runner::core
{
    void startFirmware(sabre::core::ResourceManagerConfig config,
                       const std::string &firmwareFile,
                       const std::string &entryPoint)
    {
        sabre::impl::pilot::Factory fac(nullptr);
        sabre::core::ResourceManager rm(fac, config);

        sabre_runner::core::DynamicLibrary::UniquePtr firmware =
            std::make_unique<sabre_runner::core::LinuxDynamicLibrary>(
                firmwareFile);
        firmware->getEntryPoint(entryPoint)(rm);
    }
} // namespace sabre_runner::core