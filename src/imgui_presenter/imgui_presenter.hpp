#pragma once

#include "imgui_visitor.hpp"
#include <presenter.hpp>

namespace sabre::impl::pilot
{
    enum class ImGuiTheme
    {
        DARK,
        LIGHT,
        CLASSIC
    };

    class ImGuiPresenter : public Presenter
    {
    private:
        // Essential ImGui variables
        GLFWwindow *_window = nullptr;

        std::unordered_map<std::string, bool> _deviceVisibility;

        // Concrete visitor for devices
        ImGuiVisitor _imguiVisitor{_deviceVisibility};

        // Scaling variables
        float _scale = 1.0f;
        bool _autoScale = false;

        // Window toggles
        bool _showImGuiDemo = false;
        bool _showImGuiMetrics = false;

        // Theming variables
        ImGuiTheme _currentTheme = ImGuiTheme::DARK;

        // Scaling methods
        void _setScaleToAuto();
        void _setScale(float scale);
        void _zoomIn();
        void _zoomOut();

        // Event handeling
        void _handleKeyEvents();

        // Menu creation
        void _mainMenu();
        void _mainMenuFile();
        void _mainMenuView();
        void _mainMenuViewDeviceList();
        void _mainMenuViewScale();
        void _mainMenuViewTheme();
        void _mainMenuDebug();

        // Windows
        void _device(const std::string &name, SimulatorDevice &simDevic);
        void _deviceWindows();
        void _imguiDemoWindow();
        void _imguiMetricsWindow();

        // Essential ImGui setup and loop methods
        void _createWindow();
        void _createImGuiContext();
        void _loop();
        void _destroyWindow();

    public:
        ImGuiPresenter(Simulator &simulator);
        void start() override;
    };
} // namespace sabre::impl::pilot