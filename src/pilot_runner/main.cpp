#include <CLI/CLI.hpp>
#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_runner_core/linux_dynamic_library.hpp>

int main(int argc, char *argv[])
{
    CLI::App app{"Sabre Pilot Runner"};
    argv = app.ensure_utf8(argv);

    std::string firmware_file = "";
    app.add_option(
           "firmware_file", firmware_file,
           "The firmware library file (usually a `.so` file) to load and run")
        ->required();

    std::string entry_point = "startApp";
    app.add_option("entry_point", entry_point,
                   "The entry point function to call in the firmware library "
                   "(default: \"startApp\")");

    CLI11_PARSE(app, argc, argv);

    sabre::core::ResourceManagerConfig config;
    sabre::impl::pilot::Factory fac(nullptr);
    sabre::core::ResourceManager rm(fac, config);

    sabre_pilot_runner_core::DynamicLibrary::UniquePtr firmware =
        std::make_unique<sabre_pilot_runner_core::LinuxDynamicLibrary>(
            firmware_file);
    firmware->getEntryPoint(entry_point)(rm);

    return 0;
}