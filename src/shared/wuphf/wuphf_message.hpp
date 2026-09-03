#pragma once

#include <cstdint>
#include <ipc/byte_order.hpp>
#include <ipc/types.hpp>
#include <memory>
#include <ranges>
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

        template <std::ranges::range R>
        static std::optional<std::unique_ptr<ClientHello>> decode(const R &data)
        {
            if (data.size() != 4)
            {
                return nullptr;
            }

            uint32_t id = ::ipc::byte_order::deserialize<uint32_t>(data);
            return std::optional<std::unique_ptr<ClientHello>>(
                std::make_unique<ClientHello>(id));
        }

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

        template <std::ranges::range R>
        static std::optional<std::unique_ptr<UartAppend>> decode(uint32_t id,
                                                                 const R &data)
        {
            if (data.size() < 3)
            {
                return nullptr;
            }

            uint16_t uartIndex = ::ipc::byte_order::deserialize<uint16_t>(
                data | std::views::take(2));
            std::string uartData = ::ipc::byte_order::deserializeString(
                data | std::views::drop(2));

            return std::optional<std::unique_ptr<UartAppend>>(
                std::make_unique<UartAppend>(id, uartIndex, uartData));
        }

        void accept(WuphfMessageVisitor &visitor);
        const ::ipc::BufferType getRawBytes() const noexcept override;
        const uint16_t getOpCode() const noexcept override;

        const uint16_t getUartIdx() const;
        const std::string getData() const;
    };
} // namespace sabre::ipc