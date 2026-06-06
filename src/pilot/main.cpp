#include "project.hpp"
#include <iostream>
#include <pilot_core/pilot.hpp>
#include <toml++/toml.hpp>

int main(int argc, char *argv[])
{
    sabre_pilot::Pilot sabrePilot;

    if (argc == 2)
    {
        auto data = toml::parse_file(argv[1]);

        if (auto title = data["title"].value<std::string>())
        {
            std::cout << "Opening project " << *title << "\n";
        }
        else
        {
            std::cerr << "Missing project title\n";
            return 1;
        }

        auto *devices = data["devices"].as_array();
        if (!devices)
        {
            std::cerr << "Missing or invalid 'devices' array\n";
            return 1;
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
                std::cerr
                    << "Device missing either name, library of entryPoint\n";
                continue;
            }

            std::cout << "Adding device " << *name << "\n";

            auto maxGpios = (*device)["maxGpios"].value<int64_t>();
            auto upperBoundUart = (*device)["upperBoundUart"].value<int64_t>();

            if (!maxGpios || !upperBoundUart)
            {
                std::cerr << "Device " << *name
                          << " has invalid config values.\n";
                continue;
            }

            sabre::core::ResourceManagerConfig config = {
                .maxGpios = static_cast<sabre::hal::PinNumber>(*maxGpios),
                .upperboundUart =
                    static_cast<sabre::hal::UartNumber>(*upperBoundUart),
            };

            sabrePilot.addDevice(*name, config, *library, *entryPoint);
        }
    }

    sabrePilot.run();
    return 0;
}