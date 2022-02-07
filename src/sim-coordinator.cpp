#include <iostream>

#include "check-macros.hpp"
#include "sim-coordinator.hpp"

#include <SFML/Graphics.hpp>

namespace leprechauns
{

    SimCoordinator::SimCoordinator(const bool willDisplay)
        : willDisplay_(willDisplay)
        , window_()
    {}

    void SimCoordinator::run()
    {
        if (willDisplay_)
        {
            window_.create(sf::VideoMode::getDesktopMode(), "Leprechauns", sf::Style::Fullscreen);
            M_CHECK(window_.isOpen(), "Failed to open video window.");

            const sf::Vector2u windowSize = window_.getSize();
            std::cout << "Display Resolution: " << windowSize.x << 'x' << windowSize.y << std::endl;
            displayLoop();
        }
        else
        {
            std::cout << "Running without displpay." << std::endl;
            loop();
        }
    }

    void SimCoordinator::loop() {}

    void SimCoordinator::displayLoop()
    {
        while (window_.isOpen())
        {
            sf::Event event;
            while (window_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window_.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    window_.close();
                }
            }
        }
    }

} // namespace leprechauns
