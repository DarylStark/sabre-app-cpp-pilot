#include <core/ui.hpp>

namespace sabre_ui_console
{
    class ConsoleUI : public sabre_pilot::core::UI
    {
    private:
        void _printDeviceList() const;
        void _printUartBuffers(const std::string &deviceName) const;

    public:
        ConsoleUI(sabre_pilot::core::Pilot &pilot);
        ~ConsoleUI() override;

        void start() override;
    };
} // namespace sabre_ui_console