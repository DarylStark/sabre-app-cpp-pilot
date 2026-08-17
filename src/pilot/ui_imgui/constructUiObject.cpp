#include "imgui.hpp"
#include <iostream>
#include <memory>

extern "C"
{
    sabre_pilot::core::UI::UniquePtr
    constructUIObject(sabre_pilot::core::Pilot &pilot)
    {
        return std::make_unique<sabre_pilot::ui::ImGuiUI>(pilot);
    }
}