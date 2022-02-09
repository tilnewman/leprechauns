#include "sim-coordinator.hpp"

#include "check-macros.hpp"

#include <iostream>
#include <vector>

namespace leprechauns
{

    SimCoordinator::SimCoordinator()
        : m_random()
        , m_window()
        , m_drawing()
        , m_isLazyTurn(false) // who moves first is randomized below
        , m_goldCount(10)     // always this many pots of gold on the board
        , m_lazyScore(0)
        , m_greedyScore(0)
        , m_turnCounter(0)
        , m_clock()
        , m_findRandEmptyPositionsVec()
        , m_selectPossibleMovesVec()
    {
        m_selectPossibleMovesVec.reserve(4);
    }

    void SimCoordinator::run(const bool willDisplay)
    {
        resetBoard();

        if (willDisplay)
        {
            m_window.create(sf::VideoMode::getDesktopMode(), "Leprechauns", sf::Style::Fullscreen);
            M_CHECK(m_window.isOpen(), "Failed to open video window.");

            const sf::Vector2u windowSize = m_window.getSize();
            M_LOG("Display Resolution: " << windowSize.x << 'x' << windowSize.y);

            m_window.setFramerateLimit(0);

            m_drawing.setup(m_window);

            m_clock.restart();
            displayLoop();
        }
        else
        {
            M_LOG("Running without displpay.");
            m_clock.restart();
            loop();
        }

        printFinalScores();
    }

    void SimCoordinator::printFinalScores()
    {
        const float runTimeSec = m_clock.restart().asSeconds();

        std::cout << "After " << runTimeSec << "sec and " << m_turnCounter << " turns...("
                  << (static_cast<float>(m_turnCounter) / runTimeSec) << "tps)" << std::endl;

        std::cout << "\tLazy Score:  " << m_lazyScore << std::endl;
        std::cout << "\tGreedy Score:" << m_greedyScore << std::endl;

        if (m_lazyScore == m_greedyScore)
        {
            std::cout << "\tTied for first place!" << std::endl;
        }
        else
        {
            if (m_lazyScore > m_greedyScore)
            {
                std::cout << "\tLazy wins by "
                          << util::makePercentString((m_lazyScore - m_greedyScore), m_lazyScore)
                          << std::endl;
            }
            else
            {
                std::cout << "\tGreedy wins by "
                          << util::makePercentString((m_greedyScore - m_lazyScore), m_greedyScore)
                          << std::endl;
            }
        }
    }

    void SimCoordinator::loop()
    {
        for (int i = 0; i < 10000000; ++i)
        {
            moveLeprechauns();
        }
    }

    void SimCoordinator::displayLoop()
    {
        sf::Event event;

        while (m_window.isOpen())
        {
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    m_window.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    m_window.close();
                }
            }

            moveLeprechauns();

            m_drawing.draw(m_window, m_boardMap);

            // sf::sleep(sf::milliseconds(150));
        }
    }

    void SimCoordinator::resetBoard()
    {
        m_lazyScore = 0;
        m_greedyScore = 0;
        m_isLazyTurn = m_random.boolean();

        m_boardMap.clear();

        for (int y = 0; y < m_drawing.cellCount(); ++y)
        {
            for (int x = 0; x < m_drawing.cellCount(); ++x)
            {
                m_boardMap.append(sf::Vector2i{ x, y }, Content::Empty);
            }
        }

        m_findRandEmptyPositionsVec.reserve(m_boardMap.size());

        m_boardMap.at(findRandomEmptyPosition()) = Content::Lazy;
        m_boardMap.at(findRandomEmptyPosition()) = Content::Greedy;

        // check if m_goldCount is possible given board size
        M_CHECK(
            (m_goldCount < (m_drawing.cellCount() * m_drawing.cellCount())),
            "Error:  Gold count is more than available!");

        for (int i = 0; i < m_goldCount; ++i)
        {
            placeGoldRandom();
        }
    }

    void SimCoordinator::placeGoldRandom()
    {
        const Position_t position = findRandomEmptyPosition();

        if (InvalidPosition == position)
        {
            M_LOG("Unable to place random gold because the board is full!");
            return;
        }

        m_boardMap.at(position) = m_random.fromTo<int>(1, 99);
    }

    void SimCoordinator::moveLeprechauns()
    {
        if (m_isLazyTurn)
        {
            moveLazy();
        }
        else
        {
            moveGreedy();
        }

        m_isLazyTurn = !m_isLazyTurn;

        ++m_turnCounter;
    }

    void SimCoordinator::moveLazy()
    {
        // find where lazy is
        Position_t fromPosition = InvalidPosition;

        for (const auto & pair : m_boardMap)
        {
            if (Content::Lazy == pair.second)
            {
                fromPosition = pair.first;
                break;
            }
        }

        M_CHECK((InvalidPosition != fromPosition), "Error:  Can't find Lazy on the board!");

        // find closest pot of gold
        int closestGoldDistance = (m_drawing.cellCount() * m_drawing.cellCount());
        Position_t closestGoldPosition = InvalidPosition;

        for (const auto & pair : m_boardMap)
        {
            if (pair.second > 0)
            {
                const int distance =
                    (std::abs(fromPosition.x - pair.first.x) +
                     std::abs(fromPosition.y - pair.first.y));

                if (distance < closestGoldDistance)
                {
                    closestGoldDistance = distance;
                    closestGoldPosition = pair.first;
                }
            }
        }

        M_CHECK((InvalidPosition != closestGoldPosition), "Could not move Greedy because no gold!");

        moveLeprechaun(fromPosition, selectPossibleMove(fromPosition, closestGoldPosition));
    }

    void SimCoordinator::moveGreedy()
    {
        // find biggest pot of gold to move toward and starting position
        int mostGoldAmount = 0;
        Position_t mostGoldPosition = InvalidPosition;
        Position_t fromPosition = InvalidPosition;

        for (const auto & pair : m_boardMap)
        {
            if (pair.second > mostGoldAmount)
            {
                mostGoldAmount = pair.second;
                mostGoldPosition = pair.first;
            }
            else if (Content::Greedy == pair.second)
            {
                fromPosition = pair.first;
            }
        }

        M_CHECK(
            (InvalidPosition != fromPosition),
            "ERROR:  Can't move Greedy because he is "
            "not on the board!");

        M_CHECK(
            (InvalidPosition != mostGoldPosition),
            "ERROR:  Can't move Greedy because there "
            "is no gold on the board!");

        moveLeprechaun(fromPosition, selectPossibleMove(fromPosition, mostGoldPosition));
    }

    void SimCoordinator::moveLeprechaun(const Position_t from, const Position_t to)
    {
        const int whoIsMoving = m_boardMap.at(from);
        M_CHECK((whoIsMoving < 0), "Error:  Tried to move non-leprechaun!");

        m_boardMap.at(from) = Content::Empty;

        const int whoIsMovedOn = m_boardMap.at(to);
        M_CHECK((whoIsMovedOn >= 0), "Error:  Tried to move onto other leprechaun!");

        m_boardMap.at(to) = whoIsMoving;

        // check if picked up a pot of gold
        if (whoIsMovedOn > 0)
        {
            if (Content::Lazy == whoIsMoving)
            {
                m_lazyScore += whoIsMovedOn;
            }
            else
            {
                m_greedyScore += whoIsMovedOn;
            }

            placeGoldRandom();
        }
    }

    bool SimCoordinator::isPositionOnBoard(const Position_t & pos) const
    {
        return (
            (pos.x >= 0) && (pos.x < m_drawing.cellCount()) && (pos.y >= 0) &&
            (pos.y < m_drawing.cellCount()));
    }

    bool SimCoordinator::isPositionValidToMoveOn(const Position_t & position) const
    {
        return (isPositionOnBoard(position) && (m_boardMap.at(position) >= 0));
    }

    const Position_t SimCoordinator::findRandomEmptyPosition() const
    {
        m_findRandEmptyPositionsVec.clear();

        for (const auto & pair : m_boardMap)
        {
            if (Content::Empty == pair.second)
            {
                m_findRandEmptyPositionsVec.push_back(pair.first);
            }
        }

        if (m_findRandEmptyPositionsVec.empty())
        {
            return InvalidPosition;
        }
        else
        {
            return m_random.from(m_findRandEmptyPositionsVec);
        }
    }

    void SimCoordinator::setupAllPossibleMovePositions(
        const Position_t from, std::vector<Position_t> & positions) const
    {
        positions.clear();

        // clang-format off
        const Position_t left { from.x - 1, from.y };
        const Position_t right{ from.x + 1, from.y };
        const Position_t up   { from.x,     from.y - 1 };
        const Position_t down { from.x,     from.y + 1 };

        if (isPositionValidToMoveOn(left))  positions.push_back(left);
        if (isPositionValidToMoveOn(right)) positions.push_back(right);
        if (isPositionValidToMoveOn(up))    positions.push_back(up);
        if (isPositionValidToMoveOn(down))  positions.push_back(down);
        // clang-format on

        M_CHECK((!positions.empty()), "ERROR:  There are no valid move positions on the board!");
    }

    void SimCoordinator::appendMovesToward(
        const Position_t from, const Position_t to, std::vector<Position_t> & positions) const
    {
        // clang-format off
        const Position_t left { from.x - 1, from.y };
        const Position_t right{ from.x + 1, from.y };
        const Position_t up   { from.x,     from.y - 1 };
        const Position_t down { from.x,     from.y + 1 };
        // clang-format on

        if ((to.x < from.x) && isPositionValidToMoveOn(left))
        {
            positions.push_back(left);
        }
        else if ((to.x > from.x) && isPositionValidToMoveOn(right))
        {
            positions.push_back(right);
        }

        if ((to.y < from.y) && isPositionValidToMoveOn(up))
        {
            positions.push_back(up);
        }
        else if ((to.y > from.y) && isPositionValidToMoveOn(down))
        {
            positions.push_back(down);
        }
    }

    const Position_t
        SimCoordinator::selectPossibleMove(const Position_t from, const Position_t to) const
    {
        // find moves toward closest pot of gold
        m_selectPossibleMovesVec.clear();

        appendMovesToward(from, to, m_selectPossibleMovesVec);

        // if can't move in direction desired then move in random other direction
        if (m_selectPossibleMovesVec.empty())
        {
            setupAllPossibleMovePositions(from, m_selectPossibleMovesVec);
        }

        return m_random.from(m_selectPossibleMovesVec);
    }

} // namespace leprechauns
