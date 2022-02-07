#include <stddef.h>

#include "sim-coordinator.hpp"

int main()
{
    using namespace leprechauns;

    SimCoordinator sim(true);
    sim.run();

    return EXIT_SUCCESS;
}
