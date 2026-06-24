#include "imgui.hpp"
#include <iostream>
#include <memory>

extern "C"
{
    sabre_pilot::UI::UniquePtr constructUIObject(sabre_pilot::Pilot &pilot)
    {
        return std::make_unique<sabre_ui_imgui::ImGuiUI>(pilot);
    }
}