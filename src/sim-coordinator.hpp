#ifndef LEPRECHAUNS_SIMCOORDINATOR_HPP
#define LEPRECHAUNS_SIMCOORDINATOR_HPP

#include <SFML/Window.hpp>

namespace leprechauns
{

    class SimCoordinator
    {
      public:
        SimCoordinator(const bool willDisplay);

        void run();

      private:
        void loop();
        void displayLoop();

      private:
        sf::Window window_;
        bool willDisplay_;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
