#include "project.hpp"
#include <pilot_core/pilot.hpp>

int main()
{
    sabre_pilot::Project project;
    project.devices.emplace_back(sabre_pilot::ProjectDevice{
        .config = {.maxGpios = 1, .upperboundUart = 3},
        .name = "Device01",
        .library = "../app_example_app/libapp_example_app.so",
        .entryPoint = "startDev1"});
    project.devices.emplace_back(sabre_pilot::ProjectDevice{
        .config = {.maxGpios = 1, .upperboundUart = 3},
        .name = "Device02",
        .library = "../app_example_app/libapp_example_app.so",
        .entryPoint = "startDev2"});

    sabre_pilot::Pilot sabrePilot;

    for (const auto &device : project.devices)
    {
        sabrePilot.addDevice(device.name, device.config, device.library,
                             device.entryPoint);
    }

    sabrePilot.run();
    return 0;
}