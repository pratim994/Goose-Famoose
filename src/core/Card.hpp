#pragma once  


#include <cstdint>

namespace bridge {

    enum class Suit : std::uint8_t {

        Clubs,
        Diamonds,
        Hearts,
        Spades

    };

    enum class Rank : std::uint8_t {

        Two = 2,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace

    };

    struct card {
        Suit suit;
        Rank rank;


        constexpr bool default operator==(const Card&) const = default;
    }

}