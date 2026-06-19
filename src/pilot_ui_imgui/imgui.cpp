#include "imgui.hpp"

#include <iostream>

namespace sabre_ui_imgui
{
    ImGuiUI::ImGuiUI(sabre_pilot::Pilot &device) : sabre_pilot::UI(device) {}

    ImGuiUI::~ImGuiUI() {}

    void ImGuiUI::start()
    {
        std::cout << "Starting ImGui UI...\n";
        std::flush(std::cout);
    }
} // namespace sabre_ui_imgui