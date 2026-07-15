#include "wuphf.hpp"

namespace sabre_pilot::ipc
{
    std::unique_ptr<::ipc::IpcProtocol<WuphfMessage>> Wuphf::clone()
    {
        return std::make_unique<Wuphf>();
    }

    WuphfMessage Wuphf::parseBytes(std::vector<uint8_t> &bytes)
    {
        return WuphfMessage({_mcuId});
    }
} // namespace sabre_pilot::ipc