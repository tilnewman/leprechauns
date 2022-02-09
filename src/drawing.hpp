#ifndef LEPRECHAUNS_DRAWING_HPP
#define LEPRECHAUNS_DRAWING_HPP

#include "cell.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

namespace leprechauns
{

    class Drawing
    {
      public:
        Drawing();

        void draw(
            sf::RenderWindow & window,
            const Board_t & board,
            const int lazyScore,
            const int greedyScore) const;

        void setup(sf::RenderWindow & window);
        int cellCount() const { return m_cellCount; }
        const sf::Vector2f cellSize() const { return { m_cellLength, m_cellLength }; }
        const sf::Vector2f cellWindowPosition(const Position_t & pos) const;

      private:
        void drawGrid(sf::RenderWindow & window) const;
        void drawPieces(sf::RenderWindow & window, const Board_t & board) const;

        void drawScoreBars(
            sf::RenderWindow & window, const int lazyScore, const int greedyScore) const;

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
