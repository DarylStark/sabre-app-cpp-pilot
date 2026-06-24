#include <GLFW/glfw3.h>
#include <pilot_core/ui.hpp>

namespace sabre_ui_imgui
{
    struct DeviceSettings
    {
        bool isVisible = true;
    };

    class ImGuiUI : public sabre_pilot::UI
    {
    private:
        GLFWwindow *_window = nullptr;

        // Dialog switches
        bool _showImGuiDemoWindow = false;
        bool _showImGuiMetricsWindow = false;
        bool _showAboutWindow = false;
        bool _showSettingsWindow = false;

        // GUI configuration
        bool _vSync = true;

        // Devices
        std::unordered_map<std::string, DeviceSettings> _deviceSettings;

        // Lifecycle
        void _createWindow();
        void _createImGuiContext();
        void _mainLoop();
        void _cleanup();

        // Menus
        void _mainMenu();
        void _mainMenuFile();
        void _mainMenuHelp();
        void _mainMenuHelpDebug();

        // ImGui Dialogs
        void _imguiDemoWindow();
        void _imguiMetricsWindow();

        // UI diaglos
        void _aboutWindow();
        void _settingsWindow();

        // Device dialogs
        void _deviceDialog(const std::string &deviceName);

    public:
        ImGuiUI(sabre_pilot::Pilot &pilot);
        ~ImGuiUI() override;

        void start() override;
    };
} // namespace sabre_ui_imgui