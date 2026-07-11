#include "ipc_protocol.hpp"
#include <iterator>

namespace sabre_pilot::ipc
{
    PilotIpcProtocol_v1::PilotIpcProtocol_v1(
        std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> &queue)
        : sabre_pilot::ipc::PilotIpcProtocol(queue)
    {
    }

    void PilotIpcProtocol_v1::processData()
    {
        if (_dataBuffer.size() < 4)
            return;

        uint16_t type = (_dataBuffer[0] << 8) + _dataBuffer[1];
        uint16_t length = (_dataBuffer[2] << 8) + _dataBuffer[3];

        if (_dataBuffer.size() < 4 + length)
            return;

        std::vector<uint8_t> data(_dataBuffer.begin() + 4,
                                  _dataBuffer.begin() + 4 + length);

        auto message = _factory.createMessage(type);
        _queue.push(message);
    }
} // namespace sabre_pilot::ipc