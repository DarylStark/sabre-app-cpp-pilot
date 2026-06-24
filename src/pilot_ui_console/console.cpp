#include "console.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <signal.h>
#include <thread>

namespace sabre_ui_console
{
    ConsoleUI::ConsoleUI(sabre_pilot::Pilot &pilot) : sabre_pilot::UI(pilot) {}

    ConsoleUI::~ConsoleUI() {}

    void ConsoleUI::_printDeviceList() const
    {
        for (const auto &[name, device] : _pilot.getDeviceMap())
        {
            std::cout << "- " << name;
            std::string state = "Unknown";
            switch (device->getState())
            {
            case sabre_pilot::DeviceState::Error:
                state = "Error";
                break;
            case sabre_pilot::DeviceState::Running:
                state = "Running PID: " + std::to_string(device->getPid());
                break;
            case sabre_pilot::DeviceState::Starting:
                state = "Starting PID: " + std::to_string(device->getPid());
                break;
            case sabre_pilot::DeviceState::Stopped:
                state = "Stopped";
                break;
            case sabre_pilot::DeviceState::Stopping:
                state = "Stopping";
                break;
            }
            std::cout << " [" << state << "]";
            std::cout << '\n';
        }
    }

    void ConsoleUI::_printUartBuffers(const std::string &deviceName) const
    {
        const auto &deviceMap = _pilot.getDeviceMap();
        if (deviceMap.find(deviceName) == deviceMap.end())
        {
            std::cout << "Device not found: " << deviceName << "\n";
            return;
        }

        const auto &device = deviceMap.at(deviceName);
        std::cout << "UART Buffers for device: " << deviceName << "\n";
        for (sabre::hal::UartNumber uartIdx = 0;
             uartIdx < device->getUartCount(); ++uartIdx)
        {
            std::cout << std::string(80, '-') << "\n";
            std::cout << "UART " << uartIdx << "\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << device->getUartBuffer(uartIdx) << "\n";
        }
        std::cout << std::string(80, '-') << "\n";
    }

    namespace
    {
        volatile sig_atomic_t g_stop_requested = 0;

        void handle_sigint(int)
        {
            g_stop_requested = 1;
        }
    } // namespace

    void ConsoleUI::start()
    {
        std::cout << "Starting Console UI...\n";

        // --- Linux way of handeling CTRL+C --- //
        struct sigaction sa
        {
        };
        sa.sa_handler = handle_sigint;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGINT, &sa, nullptr) == -1)
        {
            std::cerr << "Failed to install SIGINT handler\n";
            return;
        }
        if (sigaction(SIGTERM, &sa, nullptr) == -1)
        {
            std::cerr << "Failed to install SIGINT handler\n";
            return;
        }
        // --- --- //

        while (!g_stop_requested)
        {
            std::cout << "> ";
            std::string command;
            std::cin.clear();
            if (!std::getline(std::cin, command))
            {
                if (g_stop_requested)
                {
                    break;
                }

                if (std::cin.eof())
                {
                    break;
                }

                if (std::cin.fail())
                {
                    std::cin.clear();
                    continue;
                }
            }
            if (command.starts_with("run "))
            {
                std::string deviceName = command.substr(4);
                if (_pilot.getDeviceMap().find(deviceName) !=
                    _pilot.getDeviceMap().end())
                {
                    _pilot.startDevice(deviceName);
                }
                else
                {
                    std::cout << "Device not found: " << deviceName << "\n";
                }
            }
            else if (command.starts_with("stop "))
            {
                std::string deviceName = command.substr(5);
                if (_pilot.getDeviceMap().find(deviceName) !=
                    _pilot.getDeviceMap().end())
                {
                    _pilot.stopDevice(deviceName);
                }
                else
                {
                    std::cout << "Device not found: " << deviceName << "\n";
                }
            }
            else if (command.starts_with("uart "))
            {
                std::string deviceName = command.substr(5);
                if (_pilot.getDeviceMap().find(deviceName) !=
                    _pilot.getDeviceMap().end())
                {
                    _printUartBuffers(deviceName);
                }
                else
                {
                    std::cout << "Device not found: " << deviceName << "\n";
                }
            }
            else if (command == "show")
            {
                _printDeviceList();
            }
            else if (command == "exit")
            {
                std::cout << "Exiting Console UI...\n";
                break;
            }
            else
            {
                std::cout << "Unknown command: " << command << "\n";
                std::cout << "Available commands:\n";
                std::cout << "  run <device_name>  - Run a specific device\n";
                std::cout << "  stop <device_name> - Stop a specific device\n";
                std::cout << "  uart <device_name> - Show UART buffers for a "
                             "specific device\n";
                std::cout << "  show               - Show available devices\n";
                std::cout << "  exit               - Exit the console\n";
            }
        }

        std::cout << "Byebye!\n";
    }
} // namespace sabre_ui_console