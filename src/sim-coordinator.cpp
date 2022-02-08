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

    void SimCoordinator::loop() {}

    void SimCoordinator::displayLoop()
    {
        while (m_window.isOpen())
        {
            sf::Event event;
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

            m_drawing.draw(m_window, m_boardMap);
        }
    }

    void SimCoordinator::resetBoard()
    {
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

        for (int i = 0; i < 10; ++i)
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

} // namespace leprechauns
