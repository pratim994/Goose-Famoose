#pragma once

#include <cstdint>


namespace bridge {

    enum class GamePhase {
        Lobyy,
        WaitingForPlayer,
        Bidding,
        Playing,
        HandComplete
    };

    struct Gamestate {
        GamePhase phase {
            GamePhase::Lobby;
        };

        std::array<Player, 4> players{};

        PlayerId currentPlayer{};

        std::vector<Card> CurrentTrick;

        std::uint32_t trickNumber{0};

        bool connected{false};

    };  
}