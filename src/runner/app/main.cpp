#include <CLI/CLI.hpp>
#include <core/start_firmware.hpp>
#include <ipc_tcp/client.hpp>
#include <sabre_impl/core.hpp>
#include <string>
#include <thread>
#include <wuphf/wuphf.hpp>
#include <wuphf/wuphf_command.hpp>

namespace sabre_runner::ui
{
    union DeviceId
    {
        uint32_t id;
        int8_t idOctets[4];
    };
} // namespace sabre_runner::ui

int main(int argc, char *argv[])
{
    CLI::App app{"Sabre Pilot Runner"};
    argv = app.ensure_utf8(argv);

    std::string firmware;
    app.add_option(
           "firmware", firmware,
           "The firmware library file (usually a `.so` file) to load and run")
        ->required()
        ->check(CLI::ExistingFile);

    uint32_t deviceId;
    app.add_option("deviceId", deviceId,
                   "The unique ID for this device in the IPC protocol")
        ->required();

    std::string firmware_entry_point = "startApp";
    app.add_option("-e,--firmware-entry-point", firmware_entry_point,
                   "The entry point function to call in the firmware library")
        ->capture_default_str();

    std::string tcp_server_ip = "127.0.0.1";
    app.add_option("--tcp-server-ip", tcp_server_ip,
                   "The TCP server for IPC connection.")
        ->capture_default_str();

    uint16_t tcp_server_port = 8998;
    app.add_option("--tcp-server-port", tcp_server_port,
                   "The TCP port for IPC connections.")
        ->check(CLI::Range(1, 65535))
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    std::cout << "ID: " << deviceId << '\n';

    using namespace std::chrono_literals;
    using ::ipc::TcpIpcClient;
    using sabre_pilot::ipc::Wuphf;
    using sabre_pilot::ipc::WuphfCommand;

    // Protocol
    ipc::Queue<WuphfCommand::UniquePtr> ipcQueue;
    ipc::IpcProtocol::SharedPtr protocol =
        std::make_shared<Wuphf>(ipcQueue, 2048);

    ipc::IpcClient::SharedPtr client = std::make_shared<TcpIpcClient>(
        protocol, tcp_server_ip, tcp_server_port);
    client->setup();
    std::thread ipcThread([client]() { client->run(); });

    if (!client->waitForConnection())
    {
        std::cerr << "Error connecting to IPC server\n";
        client->stop();
        ipcThread.join();
        return 0;
    }

    // Send Client Hello
    sabre_runner::ui::DeviceId id;
    id.id = deviceId;
    client->sendData({0x00, 0x01, 0x00, 0x04, id.idOctets[3], id.idOctets[2],
                      id.idOctets[1], id.idOctets[0]});

    // Send Uart Append on UART 0
    client->sendData({0x01, 0x01, 0x00, 19,  0x00, 0x00, 'h', 'e',
                      'l',  'l',  'o',  ' ', 'f',  'r',  'o', 'm',
                      ' ',  'S',  'a',  'b', 'r',  'e',  '\n'});

    std::this_thread::sleep_for(1s);

    client->stop();
    ipcThread.join();

    return 0;

    // Skipping this for now, will re-add later

    sabre::core::ResourceManagerConfig config;
    config.maxGpios = 1;
    config.upperboundUart = 1;
    sabre_pilot_runner_core::startFirmware(config, firmware,
                                           firmware_entry_point);

    return 0;
}