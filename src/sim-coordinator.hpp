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

        void moveLeprechauns();
        void moveLazy();
        void moveGreedy();
        void moveLeprechaun(const Position_t from, const Position_t to);

        bool isPositionOnBoard(const Position_t & position) const;
        bool isPositionValidToMoveOn(const Position_t & position) const;
        const Position_t findRandomEmptyPosition() const;
        void placeGoldRandom();

        void setupPossibleMovePositions(
            const Position_t from, std::vector<Position_t> & positions) const;

      private:
        util::Random m_random;
        sf::RenderWindow m_window;
        Drawing m_drawing;
        Board_t m_boardMap;
        bool m_isLazyTurn;
        const int m_goldCount;
        int m_lazyScore;
        int m_greedyScore;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
