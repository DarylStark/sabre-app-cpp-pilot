#include "imgui.hpp"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
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

    void ImGuiUI::start()
    {
        std::cout << "Starting ImGui UI...\n";
        std::flush(std::cout);

        _createWindow();
        _createImGuiContext();
        _mainLoop();
        _cleanup();
    }
} // namespace sabre_ui_imgui