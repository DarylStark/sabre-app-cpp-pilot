#include "wuphf_command.hpp"

namespace sabre_pilot::ipc
{
    WuphfCommand::WuphfCommand() : _dstMcu(0) {}

    WuphfCommand::WuphfCommand(uint32_t destinationMcuId)
        : _dstMcu(destinationMcuId)
    {
    }

    void WuphfCommand::setDestionationMcuId(uint32_t mcuId)
    {
        _dstMcu = mcuId;
    }

    uint32_t WuphfCommand::getDestinationMcuId() const
    {
        return _dstMcu;
    }
} // namespace sabre_pilot::ipc