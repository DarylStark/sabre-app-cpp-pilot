#include "imgui.hpp"
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace sabre_ui_imgui
{
    ImGuiUI::ImGuiUI(sabre_pilot::Pilot &device) : sabre_pilot::UI(device) {}

    ImGuiUI::~ImGuiUI() {}

    void ImGuiUI::_createWindow()
    {
        std::clog << "Creating GLFW window..." << '\n' << std::flush;
        if (glfwInit() != GLFW_TRUE)
            return;
        _window = glfwCreateWindow(1024, 768, "Sabre Pilot", NULL, NULL);
        if (!_window)
        {
            std::cerr << "Failed to create GLFW window" << '\n' << std::flush;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);
    }

    void ImGuiUI::_createImGuiContext()
    {
        std::clog << "Creating ImGui context..." << '\n' << std::flush;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        glfwSwapInterval(_vSync ? 1 : 0);
    }

    void ImGuiUI::_mainLoop()
    {
        std::clog << "Starting Loop..." << '\n' << std::flush;
        ImGuiIO &io = ImGui::GetIO();
        while (!glfwWindowShouldClose(_window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

            // Menus
            _mainMenu();

            // Dialog windows
            _imguiDemoWindow();
            _imguiMetricsWindow();
            _aboutWindow();
            _settingsWindow();

            // Device windows
            for (const auto &[deviceName, device] : _pilot.getDeviceMap())
            {
                _deviceDialog(deviceName);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(_window);
        }
    }

    void ImGuiUI::_cleanup()
    {
        std::clog << "Destroying GLFW window..." << '\n' << std::flush;
        if (_window)
        {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
        glfwTerminate();
    }

    void ImGuiUI::_mainMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            _mainMenuFile();
            _mainMenuHelp();
            ImGui::EndMainMenuBar();
        }
    }

    void ImGuiUI::_mainMenuFile()
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Settings", nullptr, &_showSettingsWindow);
            if (ImGui::MenuItem("Exit"))
            {
                glfwSetWindowShouldClose(_window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
    }

    void ImGuiUI::_mainMenuHelp()
    {
        if (ImGui::BeginMenu("Help"))
        {
            _mainMenuHelpDebug();
            ImGui::MenuItem("About", nullptr, &_showAboutWindow);
            ImGui::EndMenu();
        }
    }

    void ImGuiUI::_mainMenuHelpDebug()
    {
        if (ImGui::BeginMenu("UI debug"))
        {
            ImGui::MenuItem("Dear UI Metrics", nullptr,
                            &_showImGuiMetricsWindow);
            ImGui::MenuItem("Dear ImGui Demo", nullptr, &_showImGuiDemoWindow);
            ImGui::EndMenu();
        }
    }

    void ImGuiUI::_imguiDemoWindow()
    {
        if (_showImGuiDemoWindow)
            ImGui::ShowDemoWindow(&_showImGuiDemoWindow);
    }

    void ImGuiUI::_imguiMetricsWindow()
    {
        if (_showImGuiMetricsWindow)
            ImGui::ShowMetricsWindow(&_showImGuiMetricsWindow);
    }

    void ImGuiUI::_aboutWindow()
    {
        if (_showAboutWindow)
        {
            ImGui::Begin("About", &_showAboutWindow);
            ImGui::Text("Sabre Pilot");
            ImGui::Text("Version 1.0.0"); // TODO: real version information
            ImGui::Text("By Daryl Stark");
            ImGui::End();
        }
    }

    void ImGuiUI::_settingsWindow()
    {
        if (_showSettingsWindow)
        {
            ImGui::Begin("Settings", &_showSettingsWindow);
            if (ImGui::Checkbox("Enable vSync", &_vSync))
            {
                if (_vSync)
                {
                    glfwSwapInterval(1);
                }
                else
                {
                    glfwSwapInterval(0);
                }
            }
            ImGui::End();
        }
    }

    void ImGuiUI::_deviceDialog(const std::string &deviceName)
    {
        auto &deviceSettings = _deviceSettings.at(deviceName);
        if (deviceSettings.isVisible)
        {
            ImGui::Begin(deviceName.c_str(), &deviceSettings.isVisible);
            if (_pilot.getDeviceMap().contains(deviceName))
            {
                auto &device = _pilot.getDeviceMap().at(deviceName);

                if (ImGui::CollapsingHeader("Device information",
                                            ImGuiTreeNodeFlags_DefaultOpen))
                {

                    if (ImGui::BeginTable("device_row", 2,
                                          ImGuiTableFlags_SizingStretchProp))
                    {
                        ImGui::TableSetupColumn(
                            "Info", ImGuiTableColumnFlags_WidthStretch);
                        ImGui::TableSetupColumn(
                            "Action", ImGuiTableColumnFlags_WidthFixed);
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text(deviceName.c_str());

                        ImGui::Text("Status unknown");

                        ImGui::EndTable();
                    }
                }

                for (uint32_t idx = 0; idx < device->getUartCount(); idx++)
                {
                    std::string title = "UART" + std::to_string(idx);
                    if (ImGui::CollapsingHeader(title.c_str(),
                                                ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        std::string child_id =
                            "UARTOutputChild" + std::to_string(idx);
                        ImGui::BeginChild(child_id.c_str(), ImVec2(0, 250),
                                          true,
                                          ImGuiWindowFlags_HorizontalScrollbar);
                        ImGui::TextWrapped("%s",
                                           device->getUartBuffer(idx).c_str());
                        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                            ImGui::SetScrollHereY(1.0f);
                        ImGui::EndChild();

                        float buttonWidth =
                            ImGui::CalcTextSize("Clear buffer").x +
                            ImGui::GetStyle().FramePadding.x * 2.0f;
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                                             ImGui::GetContentRegionAvail().x -
                                             buttonWidth);
                        std::string buttonTextAndId =
                            "Clear buffer##uart_" + std::to_string(idx);
                        if (ImGui::Button(buttonTextAndId.c_str()))
                        {
                            device->clearUartBuffer(idx);
                        }
                    }
                }
            }
            else
            {
                ImGui::Text("Device not found in the pilot.");
            }
            ImGui::End();
        }
    }

    void ImGuiUI::start()
    {
        std::cout << "Starting ImGui UI...\n";
        std::flush(std::cout);

        for (const auto &[name, device] : _pilot.getDeviceMap())
        {
            _deviceSettings[name] = DeviceSettings();
        }

        _createWindow();
        _createImGuiContext();
        _mainLoop();
        _cleanup();
    }
} // namespace sabre_ui_imgui