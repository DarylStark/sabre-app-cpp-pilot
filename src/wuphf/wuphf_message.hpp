#pragma once

#include <cstdint>

namespace sabre_pilot::ipc
{
    struct WuphfMessage
    {
        uint32_t dstMcu;
    };
} // namespace sabre_pilot::ipc