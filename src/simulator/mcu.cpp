#include "mcu.hpp"
#include "simulator.hpp"

namespace sabre::impl::pilot
{
    Mcu::Mcu(DeviceConfig config, sabre::runtime::App::UniquePtr &&app)
        : Device(config, std::move(app))
    {
    }

    void Mcu::accept(DeviceVisitor &visitor, const std::string &name)
    {
        visitor.visitMcu(*this, name);
    }
} // namespace sabre::impl::pilot