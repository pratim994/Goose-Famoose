#include "bridge/Player.h"

namespace bridge {

Player::Player(Seat seat, std::string name)
    : seat_(seat), name_(std::move(name)) {
    if (name_.empty()) {
        name_ = seatName(seat_);
    }
}

} // namespace bridge