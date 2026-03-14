#include "imgui_visitor.hpp"
#include <cstdint>
#include <imgui.h>
#include <string>

#include "mcu.hpp"
#include <simulator.hpp>
#include <sstream>

namespace sabre::impl::pilot
{
    ImGuiVisitor::ImGuiVisitor(
        std::unordered_map<std::string, bool> &deviceVisibility)
        : _deviceVisibility(deviceVisibility)
    {
    }

    void ImGuiVisitor::visitMcu(Mcu &mcu, const std::string &name)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin(name.c_str(), &_deviceVisibility[name]);

        // Calculate split
        float window_width = ImGui::GetContentRegionAvail().x;
        float window_height = ImGui::GetContentRegionAvail().y;
        float left_width = window_width * 0.7f;
        float right_width = window_width - left_width;

        // Remove spacing between children
        ImGuiStyle &style = ImGui::GetStyle();
        float old_item_spacing_x = style.ItemSpacing.x;
        float old_item_spacing_y = style.ItemSpacing.y;
        float old_window_padding_x = style.WindowPadding.x;
        float old_window_padding_y = style.WindowPadding.y;
        style.ItemSpacing.x = 0.0f;
        style.ItemSpacing.y = 0.0f;
        style.WindowPadding.x = 0.0f;
        style.WindowPadding.y = 0.0f;

        ImGui::BeginChild("LeftPane", ImVec2(left_width, window_height), true);
        for (const auto &[uart_number, uart_data] : mcu.get_uart_map())
        {
            std::string header_output =
                "UART " + std::to_string(uart_number) + " output";
            if (ImGui::CollapsingHeader(header_output.c_str(),
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {
                std::string child_id =
                    "UARTOutputChild" + std::to_string(uart_number);
                ImGui::BeginChild(child_id.c_str(), ImVec2(0, 250), true,
                                  ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::TextWrapped("%s", uart_data.outputData.c_str());
                // Auto-scroll to bottom if already at bottom
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
            }

            std::string header_input =
                "UART " + std::to_string(uart_number) + " input :: buffer";
            if (ImGui::CollapsingHeader(header_input.c_str(),
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {
                std::string child_id =
                    "UARTInputChild" + std::to_string(uart_number);
                ImGui::BeginChild(child_id.c_str(), ImVec2(0, 250), true,
                                  ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::TextWrapped("%s", uart_data.input_buffer.c_str());
                // Auto-scroll to bottom if already at bottom
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
            }

            std::string header_input_consumed =
                "UART " + std::to_string(uart_number) + " input :: consumed";
            if (ImGui::CollapsingHeader(header_input_consumed.c_str(),
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {
                std::string child_id =
                    "UARTInputConsumedChild" + std::to_string(uart_number);
                ImGui::BeginChild(child_id.c_str(), ImVec2(0, 250), true,
                                  ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::TextWrapped("%s", uart_data.inputDataConsumed.c_str());
                // Auto-scroll to bottom if already at bottom
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
            }
        }

        if (ImGui::CollapsingHeader("API logging",
                                    ImGuiTreeNodeFlags_DefaultOpen))
        {
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right region (30%)
        ImGui::BeginChild("RightPane", ImVec2(right_width, window_height),
                          true);

        if (ImGui::CollapsingHeader("System", ImGuiTreeNodeFlags_DefaultOpen))
            ImGui::Text("Here comes the system information");

        if (ImGui::CollapsingHeader("Input GPIOs",
                                    ImGuiTreeNodeFlags_DefaultOpen))
            for (const auto &input_gpio : mcu.get_gpios(GPIOType::INPUT))
            {
                char label[32];
                snprintf(label, sizeof(label), "GPIO %d", input_gpio.number);
                if (ImGui::Selectable(label, false,
                                      ImGuiSelectableFlags_SpanAllColumns))
                    mcu.set_gpio_state(input_gpio.number, !input_gpio.state);
                // Right mouse button DOWN handler
                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    mcu.set_gpio_state(input_gpio.number, !input_gpio.state);

                // Right mouse button UP handler
                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                    mcu.set_gpio_state(input_gpio.number, !input_gpio.state);
                ImGui::SameLine(
                    ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x -
                    ImGui::CalcTextSize(input_gpio.state ? "HIGH" : "LOW").x);
                if (input_gpio.state)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text,
                                          IM_COL32(0, 200, 0, 255)); // Green
                    ImGui::Text("HIGH");
                    ImGui::PopStyleColor();
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Text,
                                          IM_COL32(200, 0, 0, 255)); // Red
                    ImGui::Text("LOW");
                    ImGui::PopStyleColor();
                }
            }

        if (ImGui::CollapsingHeader("Output GPIOs",
                                    ImGuiTreeNodeFlags_DefaultOpen))
            for (const auto &output_gpio : mcu.get_gpios(GPIOType::OUTPUT))
            {
                ImGui::Text("GPIO %d", output_gpio.number);
                ImGui::SameLine(
                    ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x -
                    ImGui::CalcTextSize(output_gpio.state ? "HIGH" : "LOW").x);
                ImGui::Text(output_gpio.state ? "HIGH" : "LOW");
            }
        ImGui::EndChild();

        // Restoring spacing
        style.ItemSpacing.x = old_item_spacing_x;
        style.ItemSpacing.y = old_item_spacing_y;
        style.WindowPadding.x = old_window_padding_x;
        style.WindowPadding.y = old_window_padding_y;

        ImGui::End();
        ImGui::PopStyleVar();
    }
} // namespace sabre::impl::pilot