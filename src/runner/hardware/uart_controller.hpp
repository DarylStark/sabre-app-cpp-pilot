#pragma once

#include <functional>
#include <string>

namespace sabre_runner::hardware
{
    using UartControllerBufferCallback =
        std::function<void(const std::string &)>;

    class UartController
    {
    private:
        size_t _inputBufferSize = 0;
        size_t _outputBufferSize = 0;
        std::string _outputBuffer = "";
        std::string _inputBuffer = "";
        bool _initialized = false;
        UartControllerBufferCallback _outputBufferCallback;

    public:
        void initialize(size_t inputBufferSize, size_t outputBufferSize);
        void deinitialize();
        bool isInitialized() const;

        void setOutputBufferCallback(UartControllerBufferCallback callback);

        int write(const char data);
        void flush();

        const std::string &getOutputBuffer() const;

    private:
        void _sendOutputBufferToCallback();
        void _raiseOnUnitialized() const;
    };
} // namespace sabre_runner::hardware