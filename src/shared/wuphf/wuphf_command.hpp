#pragma once

#include <core/device.hpp>
#include <cstdint>
#include <memory>

namespace sabre::ipc
{
    class WuphfCommand
    {
    public:
        using Ptr = WuphfCommand *;
        using SharedPtr = std::shared_ptr<WuphfCommand>;
        using UniquePtr = std::unique_ptr<WuphfCommand>;

    protected:
        uint32_t _dstMcu = 0;

    public:
        WuphfCommand();
        virtual ~WuphfCommand() = default;
        WuphfCommand(uint32_t destinationMcuId);

        void setDestionationMcuId(uint32_t mcuId);
        uint32_t getDestinationMcuId() const;

        virtual void
        executeForDevice(sabre_pilot::core::Device &device) const = 0;
    };

    class ClientHello : public WuphfCommand
    {
    public:
        ClientHello(uint32_t destinationMcuId);
        void executeForDevice(sabre_pilot::core::Device &device) const;
    };

    class UartAppend : public WuphfCommand
    {
    private:
        uint16_t _uartIdx;
        const std::string _data;

    public:
        UartAppend(uint32_t destinationMcuId, uint16_t uartIdx,
                   const std::string &data);
        void executeForDevice(sabre_pilot::core::Device &device) const;
    };
} // namespace sabre::ipc