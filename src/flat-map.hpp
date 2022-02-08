#ifndef FLAT_MAP_HPP_INCLUDED
#define FLAT_MAP_HPP_INCLUDED
//
// flat-map.hpp
//
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace util
{

    // Replacement for std::map for those times when you wish it was just a vector.
    // Not sorted to favor speed, therefore linear run-time and duplicates are possible.
    template <typename key_t, typename data_t>
    class FlatMap
    {
      public:
        using value_t = std::pair<key_t, data_t>;
        using container_t = std::vector<value_t>;
        using iterator_t = typename container_t::iterator;
        using const_iterator_t = typename container_t::const_iterator;
        using reverse_iterator_t = std::reverse_iterator<iterator_t>;
        using const_reverse_iterator_t = std::reverse_iterator<const_iterator_t>;

        FlatMap()
            : m_vector()
        {}

        FlatMap(const FlatMap &) = default;
        FlatMap(FlatMap &&) = default;

        FlatMap & operator=(const FlatMap &) = default;
        FlatMap & operator=(FlatMap &&) = default;

        bool empty() const noexcept { return m_vector.empty(); }
        std::size_t size() const noexcept { return m_vector.size(); }
        void clear() noexcept { m_vector.clear(); }

        void reserve(const std::size_t count) { m_vector.reserve(count); }
        std::size_t capacity() const noexcept { return m_vector.capacity(); }
        void shrinkToFit() { m_vector.shrink_to_fit(); }

        data_t & operator[](const key_t & key)
        {
            for (value_t & pair : m_vector)
            {
                if (key == pair.first)
                {
                    return pair.second;
                }
            }

            m_vector.emplace_back(key, data_t{});
            return m_vector[m_vector.size() - 1].second;
        }

        data_t & at(const key_t & key)
        {
            for (value_t & pair : m_vector)
            {
                if (key == pair.first)
                {
                    return pair.second;
                }
            }

            throw std::out_of_range("FlatMap::at() - key not found");
        }

        const data_t & at(const key_t & key) const
        {
            for (const value_t & pair : m_vector)
            {
                if (key == pair.first)
                {
                    return pair.second;
                }
            }

            throw std::out_of_range("FlatMap::at()const - key not found");
        }

        // duplicate keys maintained
        void append(const value_t & pair) { m_vector.push_back(pair); }
        void append(const key_t & key, const data_t & data) { m_vector.emplace_back(key, data); }

        // will erase all duplicate keys
        void erase(const key_t & key)
        {
            m_vector.erase(
                std::remove_if(
                    std::begin(m_vector),
                    std::end(m_vector),
                    [&](const value_t & pair) { return (key == pair.first); }),
                std::end(m_vector));
        }

        iterator_t erase(const const_iterator_t & iter) { return m_vector.erase(iter); }

        iterator_t erase(const const_iterator_t & from, const const_iterator_t & to)
        {
            return m_vector.erase(from, to);
        }

        iterator_t find(key_t & key)
        {
            return std::find_if(
                std::begin(m_vector), std::end(m_vector), [&](const value_t & pair) {
                    return (pair.first == key);
                });
        }

        const_iterator_t find(const key_t & key) const
        {
            return std::find_if(
                std::begin(m_vector), std::end(m_vector), [&](const value_t & pair) {
                    return (pair.first == key);
                });
        }

        bool exists(const key_t & key) const { return (find(key) != std::end(m_vector)); }

        // removes all duplicate keys
        void sortAndUnique()
        {
            std::sort(std::begin(m_vector), std::end(m_vector));

            m_vector.erase(
                std::unique(
                    std::begin(m_vector),
                    std::end(m_vector),
                    [](const value_t & left, const value_t & right) {
                        return (left.first == right.first);
                    }),
                std::end(m_vector));
        }

        constexpr iterator_t begin() noexcept { return std::begin(m_vector); }
        constexpr iterator_t end() noexcept { return std::end(m_vector); }

        constexpr const_iterator_t begin() const noexcept { return std::begin(m_vector); }
        constexpr const_iterator_t end() const noexcept { return std::end(m_vector); }

        constexpr const_iterator_t cbegin() const noexcept { return begin(); }
        constexpr const_iterator_t cend() const noexcept { return end(); }

        constexpr reverse_iterator_t rbegin() noexcept { return reverse_iterator_t(end()); }
        constexpr reverse_iterator_t rend() noexcept { return reverse_iterator_t(begin()); }

        constexpr const_reverse_iterator_t rbegin() const noexcept
        {
            return const_reverse_iterator_t(end());
        }

        constexpr const_reverse_iterator_t rend() const noexcept
        {
            return const_reverse_iterator_t(begin());
        }

        constexpr const_reverse_iterator_t crbegin() const noexcept { return rbegin(); }
        constexpr const_reverse_iterator_t crend() const noexcept { return rend(); }

        // clang-format off
        template<typename T, typename U>
        friend bool
            operator==(const FlatMap<T, U> & left, const FlatMap<T, U> & right);

        template<typename T, typename U>
        friend bool
            operator<(const FlatMap<T, U> & left, const FlatMap<T, U> & right);
        // clang-format on

      private:
        container_t m_vector;
    };

    //

    template <typename key_t, typename data_t>
    bool operator==(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        if (left.size() != right.size())
        {
            return false;
        }

        typename FlatMap<key_t, data_t>::container_t leftVec{ left.m_vector };
        typename FlatMap<key_t, data_t>::container_t rightVec{ right.m_vector };

        std::sort(std::begin(leftVec), std::end(leftVec));
        std::sort(std::begin(leftVec), std::end(leftVec));

        return (leftVec == rightVec);
    }

    template <typename key_t, typename data_t>
    bool operator!=(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        return !(left == right);
    }

    template <typename key_t, typename data_t>
    bool operator<(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        typename FlatMap<key_t, data_t>::container_t leftVec{ left.m_vector };
        typename FlatMap<key_t, data_t>::container_t rightVec{ right.m_vector };

        std::sort(std::begin(leftVec), std::end(leftVec));
        std::sort(std::begin(leftVec), std::end(leftVec));

        return (leftVec < rightVec);
    }

    template <typename key_t, typename data_t>
    bool operator>(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        return (right < left);
    }

    template <typename key_t, typename data_t>
    bool operator<=(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        return !(left > right);
    }

    template <typename key_t, typename data_t>
    bool operator>=(const FlatMap<key_t, data_t> & left, const FlatMap<key_t, data_t> & right)
    {
        return !(left < right);
    }

    //

    template <typename key_t, typename data_t>
    constexpr auto begin(FlatMap<key_t, data_t> & map) noexcept
    {
        return map.begin();
    }

    template <typename key_t, typename data_t>
    constexpr auto begin(const FlatMap<key_t, data_t> & map) noexcept
    {
        return map.begin();
    }

    template <typename key_t, typename data_t>
    constexpr auto cbegin(const FlatMap<key_t, data_t> & map) noexcept
    {
        return begin(map);
    }

    template <typename key_t, typename data_t>
    constexpr auto rbegin(FlatMap<key_t, data_t> & map) noexcept
    {
        return map.rbegin();
    }

    template <typename key_t, typename data_t>
    constexpr auto rbegin(const FlatMap<key_t, data_t> & map) noexcept
    {
        return map.rbegin();
    }

    template <typename key_t, typename data_t>
    constexpr auto crbegin(const FlatMap<key_t, data_t> & map) noexcept
    {
        return rbegin(map);
    }

    template <typename key_t, typename data_t>
    constexpr auto end(FlatMap<key_t, data_t> & map) noexcept
    {
        return map.end();
    }

    template <typename key_t, typename data_t>
    constexpr auto end(const FlatMap<key_t, data_t> & map) noexcept
    {
        return map.end();
    }

    template <typename key_t, typename data_t>
    constexpr auto cend(const FlatMap<key_t, data_t> & map) noexcept
    {
        return end(map);
    }

    template <typename key_t, typename data_t>
    constexpr auto rend(FlatMap<key_t, data_t> & map) noexcept
    {
        return map.rend();
    }

    template <typename key_t, typename data_t>
    constexpr auto rend(const FlatMap<key_t, data_t> & map) noexcept
    {
        return map.rend();
    }

    template <typename key_t, typename data_t>
    constexpr auto crend(const FlatMap<key_t, data_t> & map) noexcept
    {
        return rend(map);
    }

} // namespace util

#endif // FLAT_MAP_HPP_INCLUDED
