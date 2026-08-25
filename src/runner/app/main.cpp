#include "runner_starter.hpp"
#include <string>

int main(int argc, char *argv[])
{
    sabre_pilot::runner::RunnerStarter starter(argc, argv);
    return starter.main();
}