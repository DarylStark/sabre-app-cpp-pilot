#pragma once

#include "../simulator/simulator.hpp"

namespace sabre::impl::pilot
{
    class Presenter
    {
    protected:
        Simulator &_simulator;

    public:
        Presenter(Simulator &simulator);
        virtual void start() = 0;
    };
} // namespace sabre::impl::pilot