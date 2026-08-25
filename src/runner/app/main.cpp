#include <CLI/CLI.hpp>
#include <core/runner.hpp>
#include <string>

int main(int argc, char *argv[])
{
    CLI::App app{"Sabre Pilot Runner"};
    argv = app.ensure_utf8(argv);

    std::string firmwareFile;
    app.add_option(
           "firmware", firmwareFile,
           "The firmware library file (usually a `.so` file) to load and run")
        ->required()
        ->check(CLI::ExistingFile);

    // Hardware options
    auto *appHardware = app.add_option_group("Hardware");
    uint32_t maxGpios = 1;
    appHardware
        ->add_option("--hw-max-gpios", maxGpios,
                     "The maximum number of GPIOs this device supports.")
        ->capture_default_str();

    uint32_t upperboundUart = 1;
    appHardware
        ->add_option("--hw-upperbound-uart", upperboundUart,
                     "The upperbound of UART interfaces this device supports.")
        ->capture_default_str();

    // Software options
    auto *appSoftware = app.add_option_group("Software");
    std::string firmwareEntryPoint = "startApp";
    appSoftware
        ->add_option("--sw-firmware-entry-point", firmwareEntryPoint,
                     "The entry point function to call in the firmware library")
        ->capture_default_str();

    // IPC options
    auto *appIpc = app.add_option_group("IPC with Sabre Pilot");

    std::string ipcMode = "none";
    appIpc->add_option("--ipc-mode", ipcMode, "IPC mode")
        ->check(CLI::IsMember({"none", "tcp"}))
        ->capture_default_str();

    uint32_t ipcId = 0;
    appIpc
        ->add_option("--ipc-id", ipcId,
                     "The unique ID for this device in the IPC protocol")
        ->capture_default_str();

    std::string ipcTcpServerIp = "127.0.0.1";
    appIpc
        ->add_option("--tcp-server-ip", ipcTcpServerIp,
                     "The TCP server for IPC connection.")
        ->capture_default_str();

    uint16_t ipcTcpServerPort = 8998;
    appIpc
        ->add_option("--tcp-server-port", ipcTcpServerPort,
                     "The TCP port for IPC connections.")
        ->check(CLI::Range(1, 65535))
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    // Custom validation
    const bool ipcIdRequired = ipcMode == "tcp";

    if (ipcIdRequired && !app.count("--ipc-id") || ipcId == 0)
    {
        throw CLI::ValidationError("--ipc-id",
                                   "is required when --ipc-mode is " + ipcMode);
    }

    // Configure the Configuration object for Sabre Runner Core
    using namespace sabre_runner::core;
    CoreConfig config;
    config.hardware.maxGpios = maxGpios;
    config.hardware.upperboundUart = upperboundUart;
    config.software.firmwareFile = firmwareFile;
    config.software.entryPoint = firmwareEntryPoint;
    if (ipcMode == "none")
    {
        config.ipc = IpcNoneConfig{};
    }
    else if (ipcMode == "tcp")
    {
        config.ipc = IpcTcpConfig{};
        auto &ipcConfig = std::get<IpcTcpConfig>(config.ipc);
        ipcConfig.serverIp = ipcTcpServerIp;
        ipcConfig.serverPort = ipcTcpServerPort;
        ipcConfig.deviceId = ipcId;
    }

    // Create the Runner object
    Runner runner(config);
    runner.start();

    return 0;

    // using namespace std::chrono_literals;
    // using ::ipc::tcp::TcpIpcClient;
    // using sabre::ipc::Wuphf;
    // using sabre::ipc::WuphfCommand;

    // // Protocol
    // ipc::Queue<WuphfCommand::UniquePtr> ipcQueue;
    // ipc::IpcProtocol::SharedPtr protocol =
    //     std::make_shared<Wuphf>(ipcQueue, 2048);

    // ipc::IpcClient::SharedPtr client = std::make_shared<TcpIpcClient>(
    //     protocol, ipcTcpServerIp, ipcTcpServerPort);
    // client->setup();
    // std::thread ipcThread([client]() { client->run(); });

    // if (!client->waitForConnection())
    // {
    //     std::cerr << "Error connecting to IPC server\n";
    //     client->stop();
    //     ipcThread.join();
    //     return 0;
    // }

    // // Send Client Hello
    // sabre_runner::ui::DeviceId id;
    // id.id = ipcId;
    // client->sendData({0x00, 0x01, 0x00, 0x04, id.idOctets[3], id.idOctets[2],
    //                   id.idOctets[1], id.idOctets[0]});

    // // Send Uart Append on UART 0
    // client->sendData({0x01, 0x01, 0x00, 19,  0x00, 0x00, 'h', 'e',
    //                   'l',  'l',  'o',  ' ', 'f',  'r',  'o', 'm',
    //                   ' ',  'S',  'a',  'b', 'r',  'e',  '\n'});

    // std::this_thread::sleep_for(1s);

    // client->stop();
    // ipcThread.join();

    return 0;

    // Skipping this for now, will re-add later

    // sabre::core::ResourceManagerConfig config;
    // config.maxGpios = 1;
    // config.upperboundUart = 1;
    // sabre_runner::core::startFirmware(config, firmwareFile,
    // firmwareEntryPoint);

    return 0;
}