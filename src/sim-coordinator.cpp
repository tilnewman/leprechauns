#include "sim-coordinator.hpp"

#include "check-macros.hpp"

#include <iostream>

namespace leprechauns
{

    SimCoordinator::SimCoordinator()
        : m_window()
        , m_drawing()
    {}

    void SimCoordinator::run(const bool willDisplay)
    {
        setupBoard();

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

    void SimCoordinator::setupBoard()
    {
        for (int y = 0; y < m_drawing.cellCount(); ++y)
        {
            for (int x = 0; x < m_drawing.cellCount(); ++x)
            {
                m_boardMap.append(sf::Vector2i{ x, y }, Content::Empty);
            }
        }
    }

} // namespace leprechauns
