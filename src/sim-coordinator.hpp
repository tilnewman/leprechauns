#ifndef LEPRECHAUNS_SIMCOORDINATOR_HPP
#define LEPRECHAUNS_SIMCOORDINATOR_HPP

#include "cell.hpp"
#include "drawing.hpp"
#include "random.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace leprechauns
{

    class SimCoordinator
    {
      public:
        SimCoordinator();

        void run(const bool willDisplay);

      private:
        void loop();
        void displayLoop();
        void resetBoard();
        void placeGoldRandom();
        const Position_t findRandomEmptyPosition() const;

      private:
        util::Random m_random;
        sf::RenderWindow m_window;
        Drawing m_drawing;
        Board_t m_boardMap;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
