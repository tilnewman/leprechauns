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
        void printFinalScores();
        void loop();
        void displayLoop();
        void resetBoard();
        void placeGoldRandom();

        void moveLeprechauns();
        void moveLazy();
        void moveGreedy();
        void moveLeprechaun(const Position_t from, const Position_t to);

        bool isPositionOnBoard(const Position_t & position) const;
        bool isPositionValidToMoveOn(const Position_t & position) const;
        const Position_t findRandomEmptyPosition() const;

        void setupAllPossibleMovePositions(
            const Position_t from, std::vector<Position_t> & positions) const;

        void appendMovesToward(
            const Position_t from, const Position_t to, std::vector<Position_t> & positions) const;

        const Position_t selectPossibleMove(const Position_t from, const Position_t to) const;

      private:
        util::Random m_random;
        sf::RenderWindow m_window;
        Drawing m_drawing;
        Board_t m_boardMap;
        bool m_isLazyTurn;
        const int m_goldCount;
        int m_lazyScore;
        int m_greedyScore;
        int m_turnCounter;
        sf::Clock m_clock;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_SIMCOORDINATOR_HPP
