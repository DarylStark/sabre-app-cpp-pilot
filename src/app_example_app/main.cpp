#include "main.hpp"
#include "example_app.hpp"

extern "C"
{
    int startApp(sabre::core::ResourceManager &resourceManager)
    {
        sabre::runtime::RunApp<MyApp>(resourceManager);
        return 0;
    }
}