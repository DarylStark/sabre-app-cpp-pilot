#include "main.hpp"
#include "example_app.hpp"

extern "C"
{
    int startDev1(sabre::core::ResourceManager &resourceManager)
    {
        sabre::runtime::RunApp<MyApp>(resourceManager, "DEVICE-1");
        return 0;
    }

    int startDev2(sabre::core::ResourceManager &resourceManager)
    {
        sabre::runtime::RunApp<MyApp>(resourceManager, "DEVICE-2");
        return 0;
    }

    int startDev3(sabre::core::ResourceManager &resourceManager)
    {
        sabre::runtime::RunApp<MyApp>(resourceManager, "DEVICE-3");
        return 0;
    }
}