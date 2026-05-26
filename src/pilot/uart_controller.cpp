#include "uart_controller.hpp"
#include "exceptions.hpp"

namespace sabre_pilot
{
    void UartController::initialize(size_t inputBufferSize,
                                    size_t outputBufferSize)
    {
        _inputBufferSize = inputBufferSize;
        _outputBufferSize = outputBufferSize;
        _outputBuffer.reserve(_outputBufferSize);
        _inputBuffer.reserve(_inputBufferSize);
        _initialized = true;
    }

    void UartController::deinitialize()
    {
        _outputBuffer.clear();
        _inputBuffer.clear();
        _initialized = false;
    }

    bool UartController::isInitialized() const
    {
        return _initialized;
    }

    int UartController::write(const char data)
    {
        _raiseOnUnitialized();
        _outputBuffer.push_back(data);
        if (_outputBuffer.size() == _outputBufferSize)
            flush();
        return 1;
    }

    void UartController::flush()
    {
        _raiseOnUnitialized();
        _sendOutputBufferToCallback();
        _outputBuffer.clear();
        _outputBuffer.reserve(_outputBufferSize);
    }

    const std::string &UartController::getOutputBuffer() const
    {
        return _outputBuffer;
    }

    void UartController::setOutputBufferCallback(
        UartControllerBufferCallback callback)
    {
        _outputBufferCallback = std::move(callback);
    }

    void UartController::_sendOutputBufferToCallback()
    {
        if (!_outputBufferCallback)
            return;
        for (char &c : _outputBuffer)
            _outputBufferCallback(c);
    }

    void UartController::_raiseOnUnitialized() const
    {
        if (!_initialized)
        {
            throw sabre_pilot::exceptions::
                UartControllerNotInitializedException(
                    "Uart not yet initialized");
        }
    }

} // namespace sabre_pilot