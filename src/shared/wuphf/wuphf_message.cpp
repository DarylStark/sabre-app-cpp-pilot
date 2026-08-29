#include "wuphf_message.hpp"
#include "wuphf_message_visitor.hpp"
#include <iostream> // TODO: Remove
#include <vector>

namespace sabre::ipc
{
    WuphfMessage::WuphfMessage() : _dstMcu(0) {}

    WuphfMessage::WuphfMessage(uint32_t destinationMcuId)
        : _dstMcu(destinationMcuId)
    {
    }

    void WuphfMessage::setDestionationMcuId(uint32_t mcuId)
    {
        _dstMcu = mcuId;
    }

    uint32_t WuphfMessage::getDestinationMcuId() const
    {
        return _dstMcu;
    }

    const std::vector<uint8_t> WuphfMessage::getRawBytes() const noexcept
    {
        return {};
    }

    const uint16_t WuphfMessage::getOpCode() const noexcept
    {
        return 0x0001;
    }

    ClientHello::ClientHello(uint32_t destinationMcuId)
        : WuphfMessage(destinationMcuId)
    {
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

    void ClientHello::accept(WuphfMessageVisitor &visitor)
    {
        visitor.visitClientHello(*this);
    }

    UartAppend::UartAppend(uint32_t destinationMcuId, uint16_t uartIdx,
                           const std::string &data)
        : WuphfMessage(destinationMcuId), _uartIdx(uartIdx), _data(data)
    {
    }

    void UartAppend::accept(WuphfMessageVisitor &visitor)
    {
        visitor.visitUartAppend(*this);
    }
} // namespace sabre::ipc