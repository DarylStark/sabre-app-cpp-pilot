#include "main.hpp"
#include "example_app.hpp"

extern "C"
{
    void startApp(sabre::core::ResourceManager &resourceManager)
    {
        sabre::runtime::RunApp<MyApp>(resourceManager);
    }
}