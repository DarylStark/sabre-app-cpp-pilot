#pragma once

#include <vector>

namespace ipc
{
    using BufferType = std::vector<std::byte>;
    using ConstBufferType = std::vector<const std::byte>;
} // namespace ipc