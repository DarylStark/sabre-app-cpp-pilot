#include "imgui_presenter.hpp"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <iostream>

namespace sabre::impl::pilot
{
    ImGuiPresenter::ImGuiPresenter(Simulator &simulator) : Presenter(simulator)
    {
    }

    void ImGuiPresenter::_handleKeyEvents()
    {
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
            ImGui::IsKeyPressed(ImGuiKey_Equal))
            _zoomIn();
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
            ImGui::IsKeyPressed(ImGuiKey_Minus))
            _zoomOut();
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
            ImGui::IsKeyPressed(ImGuiKey_0))
            _setScaleToAuto();
    }

    void ImGuiPresenter::_mainMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            _mainMenuFile();
            _mainMenuView();
            _mainMenuDebug();
            ImGui::EndMainMenuBar();
        }
    }

    void ImGuiPresenter::_mainMenuFile()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(_window, GLFW_TRUE);
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_mainMenuView()
    {
        if (ImGui::BeginMenu("View"))
        {
            _mainMenuViewDeviceList();
            _mainMenuViewScale();
            _mainMenuViewTheme();
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_mainMenuViewDeviceList()
    {
        if (ImGui::BeginMenu("Device"))
        {
            for (auto &device : _simulator.get_device_list())
                if (ImGui::MenuItem(device.first.c_str(), nullptr,
                                    _deviceVisibility[device.first]))
                    _deviceVisibility[device.first] =
                        !_deviceVisibility[device.first];
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_mainMenuViewScale()
    {
        if (ImGui::BeginMenu("Scale"))
        {
            if (ImGui::MenuItem("Auto", "Ctrl + 0", _autoScale))
                _setScaleToAuto();
            ImGui::Separator();
            if (ImGui::MenuItem("Zoom in", "Ctrl + =", false, _scale != 5.0f))
                _zoomIn();
            if (ImGui::MenuItem("Zoom out", "Ctrl + -", false, _scale != 1.0f))
                _zoomOut();
            ImGui::Separator();
            for (int i = 0; i <= 8; ++i)
            {
                float scale = 1.0f + i * 0.5f;
                std::string label =
                    std::to_string(static_cast<int>(scale * 100)) + "%";
                if (ImGui::MenuItem(label.c_str(), nullptr,
                                    _scale == scale && !_autoScale))
                    _setScale(scale);
            }
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_mainMenuViewTheme()
    {
        if (ImGui::BeginMenu("Theme"))
        {
            if (ImGui::MenuItem("Light", nullptr,
                                _currentTheme == ImGuiTheme::LIGHT))
                _currentTheme = ImGuiTheme::LIGHT;
            if (ImGui::MenuItem("Dark", nullptr,
                                _currentTheme == ImGuiTheme::DARK))
                _currentTheme = ImGuiTheme::DARK;
            if (ImGui::MenuItem("Classic", nullptr,
                                _currentTheme == ImGuiTheme::CLASSIC))
                _currentTheme = ImGuiTheme::CLASSIC;
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_mainMenuDebug()
    {
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("ImGui Demo Window", nullptr, &_showImGuiDemo);
            ImGui::MenuItem("ImGui Metric Window", nullptr, &_showImGuiMetrics);
            ImGui::EndMenu();
        }
    }

    void ImGuiPresenter::_device(const std::string &name,
                                 SimulatorDevice &simDevice)
    {
        if (_deviceVisibility.find(name) == _deviceVisibility.end())
            _deviceVisibility[name] = true;

        if (!_deviceVisibility[name])
            return;

        simDevice.device->accept(_imguiVisitor, name);
        return;
    }

    void ImGuiPresenter::_createWindow()
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

    void ImGuiPresenter::_createImGuiContext()
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
    }

    void ImGuiPresenter::_destroyWindow()
    {
        std::clog << "Destroying GLFW window..." << '\n' << std::flush;
        if (_window)
        {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
        glfwTerminate();
    }

    void ImGuiPresenter::_imguiDemoWindow()
    {
        if (!_showImGuiDemo)
            return;
        ImGui::ShowDemoWindow(&_showImGuiDemo);
    }

    void ImGuiPresenter::_deviceWindows()
    {
        for (auto &device : _simulator.get_device_list())
            _device(device.first, device.second);
    }

    void ImGuiPresenter::_imguiMetricsWindow()
    {
        if (!_showImGuiMetrics)
            return;
        ImGui::ShowMetricsWindow(&_showImGuiMetrics);
    }

    void ImGuiPresenter::_loop()
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

            if (_currentTheme == ImGuiTheme::DARK)
                ImGui::StyleColorsDark();
            else if (_currentTheme == ImGuiTheme::LIGHT)
                ImGui::StyleColorsLight();
            else if (_currentTheme == ImGuiTheme::CLASSIC)
                ImGui::StyleColorsClassic();

            _handleKeyEvents();
            _mainMenu();
            _deviceWindows();
            _imguiDemoWindow();
            _imguiMetricsWindow();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(_window);
        }
    }

    void ImGuiPresenter::start()
    {
        std::clog << "Starting GUI..." << '\n' << std::flush;

        _createWindow();
        _createImGuiContext();
        _setScaleToAuto();
        _loop();
        _destroyWindow();
    }

    void ImGuiPresenter::_setScaleToAuto()
    {
        if (!_window)
            return;
        float scale_x, scale_y;
        glfwGetWindowContentScale(_window, &scale_x, &scale_y);
        _setScale(scale_x);
        _autoScale = true;
    }

    void ImGuiPresenter::_setScale(float scale)
    {
        float current_scale = _scale;
        _scale = scale;
        ImGui::GetStyle().ScaleAllSizes(_scale / current_scale);
        ImGuiIO &io = ImGui::GetIO();
        io.FontGlobalScale = _scale;
        _autoScale = false;
    }

    void ImGuiPresenter::_zoomIn()
    {
        if (_scale < 5)
            _setScale(_scale + 0.5f);
    }

    void ImGuiPresenter::_zoomOut()
    {
        if (_scale >= 1.5f)
            _setScale(_scale - 0.5f);
    }
} // namespace sabre::impl::pilot