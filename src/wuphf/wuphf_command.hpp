#pragma once

#include <cstdint>

namespace sabre_pilot::ipc
{
    class WuphfCommand
    {
    protected:
        uint32_t _dstMcu = 0;

    public:
        WuphfCommand();
        WuphfCommand(uint32_t destinationMcuId);

        void setDestionationMcuId(uint32_t mcuId);
        uint32_t getDestinationMcuId() const;
    };
} // namespace sabre_pilot::ipc