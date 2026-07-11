#include "ipc_protocol.hpp"

#include <algorithm>
#include <iterator>

namespace sabre_pilot
{
    void IpcProtocol::addData(const std::vector<uint8_t> &data)
    {
        _dataBuffer.insert(_dataBuffer.end(), data.begin(), data.end());
        processData();
    }
} // namespace sabre_pilot