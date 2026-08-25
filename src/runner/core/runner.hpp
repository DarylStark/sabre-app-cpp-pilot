#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace sabre_runner::core
{
    struct HardwareConfig
    {
        uint32_t maxGpios = 1;
        uint32_t upperboundUart = 1;
    };

    struct SoftwareConfig
    {
        std::string firmwareFile{};
        std::string entryPoint{};
    };

    enum class IpcMode
    {
        None,
        Tcp
    };

    struct IpcNoneConfig
    {
    };

    struct IpcTcpConfig
    {
        std::string serverIp;
        uint16_t serverPort;
    };

    struct CoreConfig
    {
        uint32_t deviceId;
        HardwareConfig hardware;
        SoftwareConfig software;
        std::variant<IpcNoneConfig, IpcTcpConfig> ipc;
    };

    class Runner
    {
    private:
        CoreConfig _config;

    public:
        Runner(CoreConfig config);

        void start();
    };
} // namespace sabre_runner::core