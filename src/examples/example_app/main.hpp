#pragma once
#include <sabre/runtime/run_app.hpp>

extern "C"
{
    int startApp(sabre::core::ResourceManager &resourceManager);
}