#pragma once

#include "pilot.hpp"

namespace sabre_pilot::core
{
    class UI
    {
    public:
        using UniquePtr = std::unique_ptr<UI>;

    protected:
        Pilot &_pilot;

    public:
        UI(Pilot &pilot);
        virtual ~UI() = default;
        virtual void start() = 0;
    };
} // namespace sabre_pilot::core