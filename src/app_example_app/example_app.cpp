#include "example_app.hpp"

MyApp::MyApp(sabre::core::ResourceManager &resourceManager)
    : sabre::runtime::App<void>(resourceManager), _uart0(nullptr),
      _uart1(nullptr), _uart2(nullptr)
{
    _setupUarts();
}

void MyApp::_setupUarts()
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

void MyApp::run()
{
    uint32_t idx = 0;
    while (idx++ < 500)
    {
        _uart0 << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    _uart0.flush();
}