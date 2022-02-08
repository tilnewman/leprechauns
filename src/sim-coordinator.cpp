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
    {}

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
            displayLoop();
        }
        else
        {
            M_LOG("Running without displpay.");
            loop();
        }
    }

    void SimCoordinator::loop()
    {
        for (int i = 0; i < 1000000; ++i)
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

            // sf::sleep(sf::milliseconds(10));
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
    }

    void SimCoordinator::moveLazy() {}

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

        // find positions to move toward that are in the directions needed
        std::vector<Position_t> possibleMoves;

        // clang-format off
        const Position_t left { fromPosition.x - 1, fromPosition.y };
        const Position_t right{ fromPosition.x + 1, fromPosition.y };
        const Position_t up   { fromPosition.x,     fromPosition.y - 1 };
        const Position_t down { fromPosition.x,     fromPosition.y + 1 };
        // clang-format on

        if ((mostGoldPosition.x < fromPosition.x) && isPositionValidToMoveOn(left))
        {
            possibleMoves.push_back(left);
        }
        else if ((mostGoldPosition.x > fromPosition.x) && isPositionValidToMoveOn(right))
        {
            possibleMoves.push_back(right);
        }

        if ((mostGoldPosition.y < fromPosition.y) && isPositionValidToMoveOn(up))
        {
            possibleMoves.push_back(up);
        }
        else if ((mostGoldPosition.y > fromPosition.y) && isPositionValidToMoveOn(down))
        {
            possibleMoves.push_back(down);
        }

        // if can't move in direction desired then move in random other direction
        if (possibleMoves.empty())
        {
            setupPossibleMovePositions(fromPosition, possibleMoves);
        }

        moveLeprechaun(fromPosition, m_random.from(possibleMoves));
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
        std::vector<Position_t> positions;
        positions.reserve(m_boardMap.size());

        for (const auto & pair : m_boardMap)
        {
            if (Content::Empty == pair.second)
            {
                positions.push_back(pair.first);
            }
        }

        if (positions.empty())
        {
            return InvalidPosition;
        }
        else
        {
            return m_random.from(positions);
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

    void SimCoordinator::setupPossibleMovePositions(
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

} // namespace leprechauns
