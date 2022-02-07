#ifndef LEPRECHAUNS_SIMCOORDINATOR_HPP
#define LEPRECHAUNS_SIMCOORDINATOR_HPP

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

        void setupDisplayVerts();

      private:
        bool m_willDisplay;
        sf::RenderWindow m_window;
        std::vector<sf::Vertex> m_cellsQuad;
        std::vector<sf::Vertex> m_gridLines;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
