#include "wuphf_command.hpp"
#include <iostream> // TODO: Remove

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

    ClientHello::ClientHello(uint32_t destinationMcuId)
        : WuphfCommand(destinationMcuId)
    {
    }

    void ClientHello::executeForDevice(sabre_pilot::core::Device &device) const
    {
        std::clog << "HELLO FROM " << _dstMcu << "\n";
    }

    UartAppend::UartAppend(uint32_t destinationMcuId, uint16_t uartIdx,
                           const std::string &data)
        : WuphfCommand(destinationMcuId), _uartIdx(uartIdx), _data(data)
    {
    }

    void UartAppend::executeForDevice(sabre_pilot::core::Device &device) const
    {
        device.appendToUArt(_uartIdx, _data);
    }
} // namespace sabre_pilot::ipc