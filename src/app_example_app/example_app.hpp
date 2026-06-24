#pragma once

#include <ostream>
#include <sabre/runtime/app.hpp>
#include <string>
#include <thread>

class MyApp : public sabre::runtime::App<void>
{
private:
    // std::ostream _uart0;
    // std::ostream _uart1;
    // std::ostream _uart2;
    std::string _name;

    // void _setupUarts();

public:
    MyApp(sabre::core::ResourceManager &resourceManager, std::string name);
    void run() override;
};