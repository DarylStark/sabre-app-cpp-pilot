#include "wuphf.hpp"
#include <algorithm>
#include <iostream>

namespace sabre::ipc
{
    Wuphf::Wuphf(::ipc::Queue<WuphfCommand::UniquePtr> &queue,
                 std::size_t bufferSize)
        : IpcProtocol(bufferSize), _queue(queue)
    {
        _parseMethods[0x0001] = [this]() { return _parseClientHello(); };
        _parseMethods[0x0101] = [this]() { return _parseUartAppend(); };
    }

    std::size_t Wuphf::_parseOnePacket()
    {
        // Buffer should be at least 4 bytes to process
        if (_buffer.size() < 4)
        {
            return 0;
        }

        // Get the fields
        uint16_t type = _readU16_be(0);
        uint16_t length = _readU16_be(2);

        // If this is not a full packet, we have to abort
        if (_buffer.size() < 4 + length)
        {
            return 0;
        }

        const auto method = _parseMethods.find(type);
        if (method != _parseMethods.end())
        {
            std::optional<WuphfCommand::UniquePtr> message;
            message = method->second();
            if (message)
            {
                _queue.push(std::move(*message));
                return length + 4;
            }
        }
        return 0;
    }

    std::optional<WuphfCommand::UniquePtr> Wuphf::_parseClientHello()
    {
        uint16_t length = _readU16_be(2);

        if (length != 4)
        {
            return std::nullopt;
        }

        uint32_t id = _readU32_be(4);
        _mcuId = id;

        return std::make_unique<ClientHello>(id);
    }

    std::optional<WuphfCommand::UniquePtr> Wuphf::_parseUartAppend()
    {
        if (!_mcuId)
            return std::nullopt;

        uint16_t length = _readU16_be(2);
        uint16_t dataLength = length - 2;

        if (_buffer.size() < 4 + length)
        {
            return std::nullopt;
        }

        uint16_t uartId = _readU16_be(4);
        std::string data(_buffer.begin() + 6, _buffer.begin() + 6 + dataLength);

        return std::make_unique<UartAppend>(_mcuId, uartId, data);
    }

    void sendWuphfCommand(::ipc::IpcClient &client, const WuphfCommand &command)
    {
        const uint16_t opcode = command.getOpCode();
        const auto data = command.getRawBytes();

        std::vector<uint8_t> bytes(2 + 2 + data.size());
        size_t length = data.size();

        bytes[0] = (opcode & 0xff00) >> 24;
        bytes[1] = opcode & 0x00ff;

        // TODO: check if length is smaller then 2^16

        bytes[2] = (length & 0xff00) >> 24;
        bytes[3] = length & 0x00ff;

        std::copy(data.begin(), data.end(), bytes.begin() + 4);

        std::string outData(bytes.begin(), bytes.end());

        client.sendData(outData);
    }
} // namespace sabre::ipc