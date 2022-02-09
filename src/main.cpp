#include <stddef.h>

#include "sim-coordinator.hpp"

int main()
{
    using namespace leprechauns;

    SimCoordinator sim;
    sim.run(false);

    return EXIT_SUCCESS;
}
