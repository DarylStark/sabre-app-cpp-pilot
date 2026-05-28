#pragma once

#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/device.hpp>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

class MyApp : public sabre::runtime::App<void>
{
private:
    std::ostream _uart0;
    std::ostream _uart1;
    std::ostream _uart2;

    void _setupUarts();

public:
    MyApp(sabre::core::ResourceManager &resourceManager);
    void run() override;
};