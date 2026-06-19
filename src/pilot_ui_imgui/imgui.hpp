#include <GLFW/glfw3.h>
#include <pilot_core/ui.hpp>

namespace sabre_ui_imgui
{
    class ImGuiUI : public sabre_pilot::UI
    {
    private:
        GLFWwindow *_window = nullptr;

        // ImGui switches
        bool _showAboutWindow = false;
        bool _showDemoWindow = false;
        bool _showMetricsWindow = false;

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

        // Dialogs
        void _imguiDemoWindow();
        void _imguiMetricsWindow();
        void _imguiAboutWindow();

    public:
        ImGuiUI(sabre_pilot::Pilot &device);
        ~ImGuiUI() override;

        void start() override;
    };
} // namespace sabre_ui_imgui