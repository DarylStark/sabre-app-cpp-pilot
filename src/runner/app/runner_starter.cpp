#include "runner_starter.hpp"
#include <CLI/CLI.hpp>
#include <core/runner.hpp>

namespace sabre_pilot::runner
{
    RunnerStarter::RunnerStarter(int argc, char *argv[]) noexcept
        : _argc(argc), _argv(argv), _app("Sabre Pilot Runner")
    {
        _argv = _app.ensure_utf8(_argv);

        // Add all required options
        _addPositionalArgs();
        _addOptionalHardwareArgs();
        _addOptionalSoftwareArgs();
        _addOptionalIpcArgs();
    }

    void RunnerStarter::_addPositionalArgs()
    {
        // std::string firmwareFile;
        _app.add_option("firmware", _config.software.firmwareFile,
                        "The firmware library file (usually a `.so` file) to "
                        "load and run")
            ->required()
            ->check(CLI::ExistingFile);
    }

    void RunnerStarter::_addOptionalHardwareArgs()
    {
        _config.hardware.maxGpios = 1;
        _config.hardware.upperboundUart = 1;

        auto *appHardware = _app.add_option_group("Hardware");

        appHardware
            ->add_option("--hw-max-gpios", _config.hardware.maxGpios,
                         "The maximum number of GPIOs this device supports.")
            ->capture_default_str();

        appHardware
            ->add_option(
                "--hw-upperbound-uart", _config.hardware.upperboundUart,
                "The upperbound of UART interfaces this device supports.")
            ->capture_default_str();
    }

    void RunnerStarter::_addOptionalSoftwareArgs()
    {
        _config.software.entryPoint = "startApp";

        auto *appSoftware = _app.add_option_group("Software");
        appSoftware
            ->add_option(
                "--sw-firmware-entry-point", _config.software.entryPoint,
                "The entry point function to call in the firmware library")
            ->capture_default_str();
    }

    void RunnerStarter::_addOptionalIpcArgs()
    {
        _config.deviceId = 0;
        _ipcMode = "none";

        auto *appIpc = _app.add_option_group("IPC configuration");

        appIpc->add_option("--ipc-mode", _ipcMode, "IPC mode")
            ->check(CLI::IsMember({"none", "tcp"}))
            ->capture_default_str();

        appIpc
            ->add_option("--ipc-id", _config.deviceId,
                         "The unique ID for this device in the IPC protocol")
            ->capture_default_str();

        _ipc_tcp_config.serverIp = "127.0.0.1";
        appIpc
            ->add_option("--tcp-server-ip", _ipc_tcp_config.serverIp,
                         "The TCP server for IPC connection.")
            ->capture_default_str();

        _ipc_tcp_config.serverPort = 8998;
        appIpc
            ->add_option("--tcp-server-port", _ipc_tcp_config.serverPort,
                         "The TCP port for IPC connections.")
            ->check(CLI::Range(1, 65535))
            ->capture_default_str();
    }

    int RunnerStarter::_parse()
    {
        CLI11_PARSE(_app, _argc, _argv);

        const bool ipcIdRequired = _ipcMode == "tcp";

        if (ipcIdRequired && (!_app.count("--ipc-id") || _config.deviceId == 0))
        {
            throw CLI::ValidationError(
                "--ipc-id", "is required when --ipc-mode is " + _ipcMode);
        }

        if (_ipcMode == "none")
        {
            _config.ipc = sabre_runner::core::IpcNoneConfig{};
        }
        else if (_ipcMode == "tcp")
        {
            _config.ipc = _ipc_tcp_config;
        }

        return 0;
    }
    void RunnerStarter::_startRunner()
    {
        sabre_runner::core::Runner runner(_config);
        runner.start();
    }

    int RunnerStarter::main()
    {
        int retval = _parse();
        _startRunner();
        return retval;
    }
} // namespace sabre_pilot::runner