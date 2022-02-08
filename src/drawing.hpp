#ifndef LEPRECHAUNS_DRAWING_HPP
#define LEPRECHAUNS_DRAWING_HPP

#include <SFML/Graphics.hpp>

#include <vector>

namespace leprechauns
{

    class Drawing
    {
      public:
        Drawing();

        void draw(sf::RenderWindow & window);
        void setup(sf::RenderWindow & window);

      private:
        sf::Color m_backgroundColor;
        sf::Color m_cellColor;
        sf::Color m_gridColor;
        sf::Vector2f m_windowSize;
        float m_boxPad;
        float m_boxLength;
        sf::FloatRect m_boxRect;
        int m_cellCount;
        float m_cellLength;
        std::vector<sf::Vertex> m_cellsQuad;
        std::vector<sf::Vertex> m_gridLines;
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_DRAWING_HPP
