#pragma once

#include <cstdint>
#include "core/Card.hpp"
#include <string>
#include <vector>

namespace {

    using PlayerId = std::uint32_t;

    enum class Seat {
            North,
            South,
            East,
            West
    }

    struct Player {

        PlayerId id{};
        Seat seat{};

        std::string name;

        std::vector<Card> hand;
    }
}