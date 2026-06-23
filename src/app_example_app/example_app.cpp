#include "example_app.hpp"
#include <string>

MyApp::MyApp(sabre::core::ResourceManager &resourceManager, std::string name)
    : sabre::runtime::App<void>(resourceManager), _uart0(nullptr),
      _uart1(nullptr), _uart2(nullptr), _name(std::move(name))
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
    _uart0 << "UART0 works!\n" << std::flush;
    _uart1 << "UART1 works!\n" << std::flush;
    _uart2 << "UART2 works!\n" << std::flush;
    while (true)
    {
        _uart0 << _name << " - Still running :)\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
}