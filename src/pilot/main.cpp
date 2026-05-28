#include <iostream>
#include <pilot_impl/core.hpp>
#include <pilot_impl/device.hpp>
#include <sabre/runtime/app.hpp>
#include <sabre/runtime/run_app.hpp>
#include <thread>

class MyApp : public sabre::runtime::App<void>
{
private:
    std::ostream _uart0;
    std::ostream _uart1;
    std::ostream _uart2;

    void _setupUarts()
    {
        auto &txPin = this->getResourceManager().gpio().getGpio(0);
        auto &rxPin = this->getResourceManager().gpio().getGpio(1);
        this->getResourceManager().serial().configureUart(0, 9600, txPin, rxPin,
                                                          100);
        this->getResourceManager().serial().configureUart(1, 9600, txPin, rxPin,
                                                          100);
        this->getResourceManager().serial().configureUart(2, 9600, txPin, rxPin,
                                                          100);
        this->getResourceManager().serial().getUart(0).initialize();
        this->getResourceManager().serial().getUart(1).initialize();
        this->getResourceManager().serial().getUart(2).initialize();

        _uart0.rdbuf(
            &this->getResourceManager().serial().getSerialStreamBufForUart(0));

        _uart1.rdbuf(
            &this->getResourceManager().serial().getSerialStreamBufForUart(1));

        _uart2.rdbuf(
            &this->getResourceManager().serial().getSerialStreamBufForUart(2));
    }

public:
    MyApp(sabre::core::ResourceManager &resourceManager)
        : sabre::runtime::App<void>(resourceManager), _uart0(nullptr),
          _uart1(nullptr), _uart2(nullptr)
    {
        _setupUarts();
    }

    void run()
    {
        uint32_t idx = 0;
        while (idx++ < 500)
        {
            _uart0 << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        _uart0.flush();
    }
};

int main()
{
    sabre::core::ResourceManagerConfig config = {.maxGpios = 26,
                                                 .upperboundUart = 3};
    sabre::impl::pilot::Device my_device(config);
    sabre::impl::pilot::Factory factory(&my_device);
    sabre::core::ResourceManager rm(factory, config);

    std::string uartOutputBuffer[3];

    // Configure the UART devices for the `device`
    for (uint16_t idx = 0; idx < config.upperboundUart; idx++)
    {
        my_device.getUartController(idx).setOutputBufferCallback(
            [idx, &uartOutputBuffer](char b)
            { uartOutputBuffer[idx].push_back(b); });
    }

    sabre::runtime::RunApp<MyApp>(rm);

    std::cout << "UART0 --> " << uartOutputBuffer[0] << '\n';

    return 0;
}