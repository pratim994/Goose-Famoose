#pragma once
#include <string>
#include <utility>
#include "bridge/Hand.h"
#include "bridge/Seat.h"

namespace bridge {

// A Player is a seat at the table plus the cards currently in that seat's
// hand. Deliberately no notion of "human" vs. "AI" vs. "network" here --
// that distinction belongs to whatever drives input into the engine (local
// mouse input, a bot, or a Django/WebSocket message), not to the core data
// model.
class Player {
public:
    explicit Player(Seat seat, std::string name = {});

    Seat seat() const { return seat_; }
    const std::string& name() const { return name_; }

    Hand& hand() { return hand_; }
    const Hand& hand() const { return hand_; }

private:
    Seat seat_;
    std::string name_;
    Hand hand_;
};

} // namespace bridge