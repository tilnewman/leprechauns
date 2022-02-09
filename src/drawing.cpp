#include "drawing.hpp"

#include "check-macros.hpp"
#include "util.hpp"

namespace leprechauns
{

    Drawing::Drawing()
        : m_backgroundColor(sf::Color(28, 28, 28))
        , m_cellColor(sf::Color::Black)
        , m_gridColor(sf::Color(150, 150, 150))
        , m_windowSize(0.0f, 0.0f)
        , m_boxPad(0.0f)
        , m_boxLength(0.0f)
        , m_boxRect()
        , m_cellCount(20)
        , m_cellLength(0.0f)
        , m_cellsQuad()
        , m_gridLines()
    {}

    void Drawing::draw(
        sf::RenderWindow & window,
        const Board_t & board,
        const int lazyScore,
        const int greedyScore)
    {
        window.clear(m_backgroundColor);

        window.draw(&m_cellsQuad[0], m_cellsQuad.size(), sf::PrimitiveType::Quads);
        window.draw(&m_gridLines[0], m_gridLines.size(), sf::PrimitiveType::Lines);

        for (const auto & pair : board)
        {
            if (Content::Empty != pair.second)
            {
                const sf::Vector2f windowPosition = cellWindowPosition(pair.first);
                const sf::Color color = cellToColor(pair.second);

                sf::FloatRect cellRect{ windowPosition, cellSize() };
                util::scaleRectInPlace(cellRect, 0.85f);

                util::drawRectangleShape(window, cellRect, true, color);
            }
        }

        const float scoreRectWidth = (m_cellLength * 0.3f);

        sf::FloatRect lazyScoreRect{ m_boxRect.left - (scoreRectWidth * 2.0f),
                                     m_boxRect.top,
                                     scoreRectWidth,
                                     m_boxRect.height };

        util::scaleRectInPlace(lazyScoreRect, { 0.7f, 1.0f });

        util::drawRectangleShape(window, lazyScoreRect, true, cellToColor(Content::Lazy));

        sf::FloatRect greedyScoreRect{
            lazyScoreRect.left - scoreRectWidth, m_boxRect.top, scoreRectWidth, m_boxRect.height
        };

        util::scaleRectInPlace(greedyScoreRect, { 0.7f, 1.0f });

        util::drawRectangleShape(window, greedyScoreRect, true, cellToColor(Content::Greedy));

        window.display();
    }

    void Drawing::setup(sf::RenderWindow & window)
    {
        m_windowSize = sf::Vector2f{ window.getSize() };
        m_boxPad = 0.05f;
        m_boxLength = (static_cast<float>(m_windowSize.y) * (1.0f - (m_boxPad * 2.0f)));

        m_boxRect.left = ((m_windowSize.x * 0.5f) - (m_boxLength * 0.5f));
        m_boxRect.top = (m_windowSize.y * m_boxPad);
        m_boxRect.width = m_boxLength;
        m_boxRect.height = m_boxLength;

        m_cellLength = (m_boxLength / static_cast<float>(m_cellCount));

        //
        // setup cell backgrounds

        const sf::Vector2f pos = util::position(m_boxRect);

        m_cellsQuad.push_back(sf::Vertex{ { pos.x, pos.y }, m_cellColor });
        m_cellsQuad.push_back(sf::Vertex{ { pos.x + m_boxLength, pos.y }, m_cellColor });

        m_cellsQuad.push_back(
            sf::Vertex{ { pos.x + m_boxLength, pos.y + m_boxLength }, m_cellColor });

        m_cellsQuad.push_back(sf::Vertex{ { pos.x, pos.y + m_boxLength }, m_cellColor });

        //
        // setup gridlines
        for (int i = 0; i <= m_cellCount; ++i)
        {
            const float vertALeft = (pos.x + (m_cellLength * static_cast<float>(i)));
            const float vertATop = pos.y;
            const sf::Vertex vertA{ { vertALeft, vertATop }, m_gridColor };
            m_gridLines.push_back(vertA);

            const float vertBLeft = vertALeft;
            const float vertBTop = (pos.y + m_boxLength);
            const sf::Vertex vertB{ { vertBLeft, vertBTop }, m_gridColor };
            m_gridLines.push_back(vertB);

            const float vertCLeft = (pos.x);
            const float vertCTop = (pos.y + (m_cellLength * static_cast<float>(i)));
            const sf::Vertex vertC{ { vertCLeft, vertCTop }, m_gridColor };
            m_gridLines.push_back(vertC);

            const float vertDLeft = (pos.x + m_boxLength);
            const float vertDTop = vertCTop;
            const sf::Vertex vertD{ { vertDLeft, vertDTop }, m_gridColor };
            m_gridLines.push_back(vertD);
        }
    }

    const sf::Vector2f Drawing::cellWindowPosition(const Position_t & pos) const
    {
        return { m_boxRect.left + (static_cast<float>(pos.x) * m_cellLength),
                 m_boxRect.top + (static_cast<float>(pos.y) * m_cellLength) };
    }

} // namespace leprechauns
