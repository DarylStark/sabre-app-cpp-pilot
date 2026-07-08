#include <CLI/CLI.hpp>
#include <pilot_impl/core.hpp>
#include <pilot_runner_core/start_firmware.hpp>
#include <pilot_runner_core/tcp_ipc_client.hpp>
#include <string>
#include <thread>

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

    using namespace std::chrono_literals;

    std::shared_ptr<sabre_pilot_runner_core::IpcClient> client =
        std::make_shared<sabre_pilot_runner_core::TcpIpcClient>(
            tcp_server_ip, tcp_server_port);
    client->setup();
    std::thread ipcThread([client]() { client->start(); });

    if (!client->waitForConnection())
    {
        std::cerr << "Error connecting to IPC server\n";
        client->stop();
        ipcThread.join();
        return 0;
    }

    std::this_thread::sleep_for(1s);
    client->sendData("Hallo!\n");
    std::cout << "Send data\n";

    std::this_thread::sleep_for(5s);
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