#pragma once

#include <cstdint>
#include <ipc/types.hpp>
#include <memory>
#include <vector>

namespace sabre::ipc
{
    class WuphfMessageVisitor; // Forward declaration

    class WuphfMessage
    {
    public:
        using Ptr = WuphfMessage *;
        using SharedPtr = std::shared_ptr<WuphfMessage>;
        using UniquePtr = std::unique_ptr<WuphfMessage>;

    protected:
        uint32_t _dstMcu = 0;

    public:
        WuphfMessage();
        virtual ~WuphfMessage() = default;
        WuphfMessage(uint32_t destinationMcuId);

        void setDestionationMcuId(uint32_t mcuId);
        uint32_t getDestinationMcuId() const;

        virtual const ::ipc::BufferType getRawBytes() const noexcept = 0;
        virtual const uint16_t getOpCode() const noexcept = 0;

        virtual void accept(WuphfMessageVisitor &visitor) = 0;
    };

    class ClientHello : public WuphfMessage
    {
    public:
        ClientHello(uint32_t destinationMcuId);
        const ::ipc::BufferType getRawBytes() const noexcept override;
        const uint16_t getOpCode() const noexcept override;
        void accept(WuphfMessageVisitor &visitor);
    };

    class UartAppend : public WuphfMessage
    {
    private:
        uint16_t _uartIdx;
        const std::string _data;

    public:
        UartAppend(uint32_t destinationMcuId, uint16_t uartIdx,
                   const std::string &data);

        void accept(WuphfMessageVisitor &visitor);
        const ::ipc::BufferType getRawBytes() const noexcept override;
        const uint16_t getOpCode() const noexcept override;

        const uint16_t getUartIdx() const;
        const std::string getData() const;
    };
} // namespace sabre::ipc