#include "imgui.hpp"
#include <iostream>

extern "C"
{
    void startUI(sabre_pilot::Pilot &device)
    {
        sabre_ui_imgui::ImGuiUI gui(device);
        gui.start();
    }
}