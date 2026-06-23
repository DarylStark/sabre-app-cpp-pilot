#include "console.hpp"
#include <iostream>

extern "C"
{
    void startUI(sabre_pilot::Pilot &device)
    {
        sabre_ui_console::ConsoleUI ui(device);
        ui.start();
    }
}