#include "config.hpp"
#include <CLI/CLI.hpp>
#include <dlfcn.h>
#include <iostream>
#include <pilot_core/linux_subprocess_strategy.hpp>
#include <pilot_core/pilot.hpp>
#include <pilot_ui_console/console.hpp>
#include <toml++/toml.hpp>

bool parseProjectTomlFile(const std::string &filename,
                          sabre_pilot::Pilot &pilot)
{
    auto data = toml::parse_file(filename);

    if (auto title = data["title"].value<std::string>())
    {
        std::cout << "Opening project " << *title << "\n";
    }
    else
    {
        std::cerr << "Missing project title\n";
        return false;
    }

    auto *devices = data["devices"].as_array();
    if (!devices)
    {
        std::cerr << "Missing or invalid 'devices' array\n";
        return false;
    }

    for (const auto &elem : *devices)
    {
        auto *device = elem.as_table();
        if (!device)
        {
            continue;
        }

        auto name = (*device)["name"].value<std::string>();
        auto library = (*device)["library"].value<std::string>();
        auto entryPoint = (*device)["entryPoint"].value<std::string>();
        if (!name || !library || !entryPoint)
        {
            std::cerr << "Device missing either name, library of entryPoint\n";
            continue;
        }

        std::cout << "Adding device " << *name << "\n";

        auto maxGpios = (*device)["maxGpios"].value<int64_t>();
        auto upperBoundUart = (*device)["upperBoundUart"].value<int64_t>();

        if (!maxGpios || !upperBoundUart)
        {
            std::cerr << "Device " << *name << " has invalid config values.\n";
            continue;
        }

        sabre::core::ResourceManagerConfig config = {
            .maxGpios = static_cast<sabre::hal::PinNumber>(*maxGpios),
            .upperboundUart =
                static_cast<sabre::hal::UartNumber>(*upperBoundUart),
        };

        pilot.addDevice(*name, config, *library, *entryPoint);
    }

    return true;
}

int main(int argc, char *argv[])
{
    CLI::App app{"Sabre Pilot"};
    argv = app.ensure_utf8(argv);

    std::string ui_lib = "";
    app.add_option("--ui-lib", ui_lib, "The UI library to use");

    std::string runnerExec = sabre_pilot::config::runnerExec;
    app.add_option("--runner-exec", runnerExec, "The executable runner");

    std::string project_file;
    app.add_option("project_file", project_file, "Path to a project TOML file");

    CLI11_PARSE(app, argc, argv);

    sabre_pilot::LinuxSubprocessStrategy strategy;
    sabre_pilot::Pilot sabrePilot(strategy, runnerExec);

    if (!project_file.empty())
    {
        // TODO: Move the parsing of the file to `pilot_core`
        parseProjectTomlFile(project_file, sabrePilot);
    }

    sabre_pilot::UI::UniquePtr ui;

    if (ui_lib.empty())
    {
        ui = std::make_unique<sabre_ui_console::ConsoleUI>(sabrePilot);
    }
    else
    {
        void *handle = dlopen(ui_lib.c_str(), RTLD_NOW);
        auto ep = reinterpret_cast<std::unique_ptr<sabre_pilot ::UI> (*)(
            sabre_pilot::Pilot &)>(dlsym(handle, "constructUIObject"));
        ui = ep(sabrePilot);
    }

    ui->start();

    return 0;
}