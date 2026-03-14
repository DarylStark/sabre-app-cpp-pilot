#include "imgui_presenter/imgui_presenter.hpp"
#include "mcu.hpp"
#include "sabre/runtime/app.hpp"
#include "simulator/device_connector.hpp"
#include "simulator/simulator.hpp"
#include <iostream>
#include <thread>

class MyApp : public sabre::runtime::App
{
private:
    sabre::hal::Serial::UniquePtr _uart0;
    bool _stop = false;

public:
    MyApp() {}

    void start() override
    {
        _uart0 = _factory->createUartObject(0, 9600, 1, 2, 512);
        _uart0->initialize();

        while (true)
        {
            std::string received_data = _uart0->readBytes(1, 16);
            for (const auto &b : received_data)
                _uart0->writeByte(b);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};

class MyGpsApp : public sabre::runtime::App
{
public:
    void start() override
    {
        // Simulate GPS data output
        auto uart1 = _factory->createUartObject(0, 9600, 0, 1, 512);
        uart1->initialize();
        while (true)
        {
            std::string nmea_sentence = "$GPGGA,123519,4807.038,N,01131.000,E,"
                                        "1,08,0.9,545.4,M,46.9,M,,*47\n";
            for (const char &p : nmea_sentence)
            {
                uart1->writeByte(p);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main()
{
    using namespace sabre::impl::pilot;

    DeviceConfig config_mcu{.gpioCount = 32, .uartCount = 2};
    DeviceConfig config_gps{.gpioCount = 2, .uartCount = 1};

    Simulator simulator;

    auto mcu =
        simulator.add_mcu("ESP32-S3", config_mcu, std::make_unique<MyApp>());
    auto gps =
        simulator.add_mcu("GPS", config_gps, std::make_unique<MyGpsApp>());
    UartConnector uart_connector(*gps, 0, *mcu, 0);

    simulator.start_device("ESP32-S3");
    simulator.start_device("GPS");

    ImGuiPresenter presenter(simulator);

    presenter.start();

    return 0;
}