#include "wuphf_command.hpp"
#include <iostream> // TODO: Remove
#include <vector>

namespace sabre::ipc
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

    const std::vector<uint8_t> WuphfCommand::getRawBytes() const noexcept
    {
        return {};
    }

    const uint16_t WuphfCommand::getOpCode() const noexcept
    {
        return 0x0001;
    }

    ClientHello::ClientHello(uint32_t destinationMcuId)
        : WuphfCommand(destinationMcuId)
    {
    }

    void ClientHello::executeForDevice(sabre_pilot::core::Device &device) const
    {
        std::clog << "HELLO FROM " << _dstMcu << "\n";
    }

    const std::vector<uint8_t> ClientHello::getRawBytes() const noexcept
    {
        // TODO: Make sure this ID is correct since it is wrong now
        uint8_t lastoctet = static_cast<uint8_t>(_dstMcu & 0x000000ff);
        return {0x00, 0x00, 0x00, lastoctet};
    }

    const uint16_t ClientHello::getOpCode() const noexcept
    {
        return 0x0001;
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
} // namespace sabre::ipc