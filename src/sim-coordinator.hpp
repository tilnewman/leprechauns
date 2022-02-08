#ifndef LEPRECHAUNS_SIMCOORDINATOR_HPP
#define LEPRECHAUNS_SIMCOORDINATOR_HPP

#include "drawing.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

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
        bool m_willDisplay;
        sf::RenderWindow m_window;
        Drawing m_drawing;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
