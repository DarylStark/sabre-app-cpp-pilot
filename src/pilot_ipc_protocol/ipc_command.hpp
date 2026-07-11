#pragma once

namespace sabre_pilot::ipc
{
    class IpcCommand
    {
    public:
        virtual void execute() = 0;
    };
} // namespace sabre_pilot::ipc