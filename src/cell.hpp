#ifndef LEPRECHAUNS_CELL_HPP
#define LEPRECHAUNS_CELL_HPP

#include "flat-map.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

namespace leprechauns
{

    enum Content
    {
        Lazy = -2,
        Greedy = -1,
        Empty = 0,
        Gold = 1
    };

    inline const std::string cellToString(const int value)
    {
        if (Content::Lazy == value)
        {
            return "Lazy";
        }
        else if (Content::Greedy == value)
        {
            return "Greedy";
        }
        else if (Content::Empty == value)
        {
            return "Empty";
        }
        else
        {
            return std::to_string(value) + "Gold";
        }
    }

    inline const sf::Color cellToColor(const int value)
    {
        if (Content::Lazy == value)
        {
            return sf::Color(50, 50, 255);
        }
        else if (Content::Greedy == value)
        {
            return sf::Color(200, 0, 200);
        }
        else if (Content::Empty == value)
        {
            return sf::Color::Transparent;
        }
        else
        {
            return sf::Color(200, 200, 0);
        }
    }

    using Position_t = sf::Vector2i;
    using Board_t = util::FlatMap<Position_t, int>;

    const Position_t InvalidPosition = { -1, -1 };

} // namespace leprechauns

#endif // LEPRECHAUNS_CELL_HPP
