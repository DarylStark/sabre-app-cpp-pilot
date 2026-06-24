#include <sabre/core/resource_manager.hpp>
#include <sabre/runtime/run_app.hpp>

extern "C"
{
    int startApp_0(sabre::core::ResourceManager &resourceManager)
    {
        return 0;
    }

    int startApp_1(sabre::core::ResourceManager &resourceManager)
    {
        return 1;
    }

    int startApp_42(sabre::core::ResourceManager &resourceManager)
    {
        return 42;
    }
}