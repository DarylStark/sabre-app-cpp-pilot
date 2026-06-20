#include "console.hpp"

#include <iostream>

namespace sabre_ui_console
{
    ConsoleUI::ConsoleUI(sabre_pilot::Pilot &device) : sabre_pilot::UI(device)
    {
    }

    ConsoleUI::~ConsoleUI() {}

    void ConsoleUI::_printDeviceList() const
    {
        std::cout << "Available devices:\n";
        for (const auto &[name, device] : _pilot.getDeviceMap())
        {
            std::cout << "- " << name << " [" << device.library << ":"
                      << device.entryPoint << "]";
            if (device.threadPtr && device.threadPtr->joinable())
            {
                std::cout << " (running)";
            }
            else
            {
                std::cout << " (stopped)";
            }
            std::cout << "\n";
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
             uartIdx < device.device->getUartCount(); ++uartIdx)
        {
            std::cout << std::string(80, '-') << "\n";
            std::cout << "UART " << uartIdx << "\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << device.device->getUartBuffer(uartIdx) << "\n";
        }
        std::cout << std::string(80, '-') << "\n";
    }

    void ConsoleUI::start()
    {
        std::cout << "Starting Console UI...\n";

        while (true)
        {
            std::cout << "> ";
            std::string command;
            std::cin.clear();
            std::getline(std::cin, command);
            if (command.starts_with("run "))
            {
                std::string deviceName = command.substr(4);
                if (_pilot.getDeviceMap().find(deviceName) !=
                    _pilot.getDeviceMap().end())
                {
                    _pilot.runDevice(deviceName);
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
                std::cout << "  uart <device_name> - Show UART buffers for a "
                             "specific device\n";
                std::cout << "  show               - Show available devices\n";
                std::cout << "  exit               - Exit the console\n";
            }
        }
    }
} // namespace sabre_ui_console