#include "wuphf_message.hpp"
#include "wuphf_message_visitor.hpp"
#include <algorithm>
#include <iostream> // TODO: Remove
#include <ipc/byte_order.hpp>
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

    ClientHello::ClientHello(uint32_t destinationMcuId)
        : WuphfMessage(destinationMcuId)
    {
    }

    const ::ipc::BufferType ClientHello::serializeObj() const noexcept
    {
        using namespace ::ipc::byte_order;
        return serialize(_dstMcu);
    }

    void ClientHello::accept(std::shared_ptr<::ipc::IpcSession> session,
                             WuphfMessageVisitor &visitor)
    {
        // Do nothing, no visitor for this class.
    }

    UartAppend::UartAppend(uint32_t destinationMcuId, uint16_t uartIdx,
                           const std::string &data)
        : WuphfMessage(destinationMcuId), _uartIdx(uartIdx), _data(data)
    {
    }

    void UartAppend::accept(std::shared_ptr<::ipc::IpcSession> session,
                            WuphfMessageVisitor &visitor)
    {
        visitor.visitUartAppend(*this);
    }

    const uint16_t UartAppend::getUartIdx() const
    {
        return _uartIdx;
    }

    const std::string UartAppend::getData() const
    {
        return _data;
    }

    const ::ipc::BufferType UartAppend::serializeObj() const noexcept
    {
        using namespace ::ipc::byte_order;
        ::ipc::BufferType output(_data.size() + 2);
        std::ranges::copy(serialize(_uartIdx), output.begin());
        std::ranges::copy(serialize(_data), output.begin() + 2);
        return output;
    }

    ServerHello::ServerHello(uint32_t destinationMcuId)
        : WuphfMessage(destinationMcuId)
    {
    }

    const ::ipc::BufferType ServerHello::serializeObj() const noexcept
    {
        using namespace ::ipc::byte_order;
        return serialize(_dstMcu);
    }

    void ServerHello::accept(std::shared_ptr<::ipc::IpcSession> session,
                             WuphfMessageVisitor &visitor)
    {
        // TODO: Implement
    }

    BindSession::BindSession(std::shared_ptr<::ipc::IpcSession> session,
                             uint32_t destinationMcuId)
        : WuphfMessage(destinationMcuId), _session(std::move(session))
    {
    }

    const ::ipc::BufferType BindSession::serializeObj() const noexcept
    {
        // TODO: Custom exception
        return {};
    }

    void BindSession::accept(std::shared_ptr<::ipc::IpcSession> session,
                             WuphfMessageVisitor &visitor)
    {
        visitor.visitBindSession(*this);
    }

    std::shared_ptr<::ipc::IpcSession> BindSession::getSession() const
    {
        return _session;
    }
} // namespace sabre::ipc