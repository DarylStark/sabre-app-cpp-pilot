#pragma once

#include <cstdint>
#include <memory>

namespace sabre_pilot::ipc
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
        WuphfCommand(uint32_t destinationMcuId);

        void setDestionationMcuId(uint32_t mcuId);
        uint32_t getDestinationMcuId() const;
    };
} // namespace sabre_pilot::ipc