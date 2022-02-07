#include "sim-coordinator.hpp"

#include "check-macros.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

namespace leprechauns
{

    SimCoordinator::SimCoordinator(const bool willDisplay)
        : m_willDisplay(willDisplay)
        , m_window()
        , m_cellsQuad()
        , m_gridLines()
    {}

    void SimCoordinator::run()
    {
        if (m_willDisplay)
        {
            m_window.create(sf::VideoMode::getDesktopMode(), "Leprechauns", sf::Style::Fullscreen);
            M_CHECK(m_window.isOpen(), "Failed to open video window.");

            const sf::Vector2u windowSize = m_window.getSize();
            M_LOG("Display Resolution: " << windowSize.x << 'x' << windowSize.y);

            m_window.setFramerateLimit(0);

            setupDisplayVerts();
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

            m_window.clear(sf::Color(33, 33, 33));

            m_window.draw(&m_cellsQuad[0], m_cellsQuad.size(), sf::PrimitiveType::Quads);
            m_window.draw(&m_gridLines[0], m_gridLines.size(), sf::PrimitiveType::Lines);

            m_window.display();
        }
    }

    void SimCoordinator::setupDisplayVerts()
    {

        //
        // setup cell backgrounds
        const sf::Color cellColor = sf::Color::Black;

        const sf::Vector2f windowSize{ m_window.getSize() };

        const float pad = 0.05f;
        const float length = (static_cast<float>(windowSize.y) * (1.0f - (pad * 2.0f)));
        const sf::Vector2f size{ length, length };

        const float left = ((windowSize.x * 0.5f) - (length * 0.5f));
        const float top = (windowSize.y * pad);
        const sf::Vector2f position{ left, top };

        m_cellsQuad.push_back(sf::Vertex{ { left, top }, cellColor });
        m_cellsQuad.push_back(sf::Vertex{ { left + length, top }, cellColor });
        m_cellsQuad.push_back(sf::Vertex{ { left + length, top + length }, cellColor });
        m_cellsQuad.push_back(sf::Vertex{ { left, top + length }, cellColor });

        //
        // setup gridlines
        const sf::Color gridColor = sf::Color(150, 150, 150);

        const int cellCount = 20;
        const float cellLength = (length / static_cast<float>(cellCount));

        for (int i = 0; i <= cellCount; ++i)
        {
            const float vertALeft = (left + (cellLength * static_cast<float>(i)));
            const float vertATop = top;
            const sf::Vertex vertA{ { vertALeft, vertATop }, gridColor };
            m_gridLines.push_back(vertA);

            const float vertBLeft = vertALeft;
            const float vertBTop = (top + length);
            const sf::Vertex vertB{ { vertBLeft, vertBTop }, gridColor };
            m_gridLines.push_back(vertB);

            const float vertCLeft = (left);
            const float vertCTop = (top + (cellLength * static_cast<float>(i)));
            const sf::Vertex vertC{ { vertCLeft, vertCTop }, gridColor };
            m_gridLines.push_back(vertC);

            const float vertDLeft = (left + length);
            const float vertDTop = vertCTop;
            const sf::Vertex vertD{ { vertDLeft, vertDTop }, gridColor };
            m_gridLines.push_back(vertD);
        }
    }

} // namespace leprechauns
