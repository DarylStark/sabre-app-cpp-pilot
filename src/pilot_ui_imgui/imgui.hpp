#include <pilot_core/ui.hpp>

namespace sabre_ui_imgui
{
    class ImGuiUI : public sabre_pilot::UI
    {
    public:
        ImGuiUI(sabre_pilot::Pilot &device);
        ~ImGuiUI() override;

        void start() override;
    };
} // namespace sabre_ui_imgui