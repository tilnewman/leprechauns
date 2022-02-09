#include "drawing.hpp"

#include "check-macros.hpp"
#include "util.hpp"

namespace leprechauns
{

    Drawing::Drawing()
        : m_backgroundColor(sf::Color(28, 28, 28))
        , m_cellColor(sf::Color::Black)
        , m_gridColor(sf::Color(50, 50, 50))
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
        const int greedyScore) const
    {
        window.clear(m_backgroundColor);

        drawGrid(window);
        drawPieces(window, board);
        drawScoreBars(window, lazyScore, greedyScore);

        window.display();
    }

    void Drawing::drawGrid(sf::RenderWindow & window) const
    {
        window.draw(&m_cellsQuad[0], m_cellsQuad.size(), sf::PrimitiveType::Quads);
        window.draw(&m_gridLines[0], m_gridLines.size(), sf::PrimitiveType::Lines);
    }

    void Drawing::drawPieces(sf::RenderWindow & window, const Board_t & board) const
    {
        for (const auto & pair : board)
        {
            if (Content::Empty != pair.second)
            {
                const sf::Vector2f windowPosition = cellWindowPosition(pair.first);
                sf::Color color = cellToColor(pair.second);

                // if a cell with gold then vary the color alpha with gold amount
                if (pair.second > 0)
                {
                    const float goldRatio{ static_cast<float>(pair.second) /
                                           static_cast<float>(99) };

                    const float goldColorValue{ goldRatio * 240.0f };

                    color.a = 15 + static_cast<unsigned char>(goldColorValue);
                }

                sf::FloatRect cellRect{ windowPosition, cellSize() };
                util::scaleRectInPlace(cellRect, 0.85f);

                util::drawRectangleShape(window, cellRect, true, color);
            }
        }
    }

    void Drawing::drawScoreBars(
        sf::RenderWindow & window, const int lazyScore, const int greedyScore) const
    {
        const float scoreRectWidth = (m_cellLength * 0.333f);

        sf::FloatRect lazyScoreRect{ m_boxRect.left - (scoreRectWidth * 2.0f),
                                     m_boxRect.top,
                                     scoreRectWidth,
                                     m_boxRect.height };

        sf::FloatRect greedyScoreRect{
            lazyScoreRect.left - scoreRectWidth, m_boxRect.top, scoreRectWidth, m_boxRect.height
        };

        if (lazyScore > greedyScore)
        {
            const float ratio{ static_cast<float>(greedyScore) / static_cast<float>(lazyScore) };
            const float move{ greedyScoreRect.height - (greedyScoreRect.height * ratio) };
            greedyScoreRect.height *= ratio;
            greedyScoreRect.top += move;
        }

        if (greedyScore > lazyScore)
        {
            const float ratio{ static_cast<float>(lazyScore) / static_cast<float>(greedyScore) };
            const float move{ lazyScoreRect.height - (lazyScoreRect.height * ratio) };
            lazyScoreRect.height *= ratio;
            lazyScoreRect.top += move;
        }

        util::scaleRectInPlace(lazyScoreRect, { 0.7f, 1.0f });

        if (lazyScore > 0)
        {
            util::drawRectangleShape(window, lazyScoreRect, true, cellToColor(Content::Lazy));
        }

        util::scaleRectInPlace(greedyScoreRect, { 0.7f, 1.0f });

        if (greedyScore > 0)
        {
            util::drawRectangleShape(window, greedyScoreRect, true, cellToColor(Content::Greedy));
        }
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
