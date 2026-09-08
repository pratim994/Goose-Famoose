#pragma once
#include <array>
#include <cstdint>

namespace bridge {

// The four positions at a bridge table. Turn order is always clockwise:
// North -> East -> South -> West -> North ...
enum class Seat : uint8_t { North = 0, East = 1, South = 2, West = 3 };

enum class Partnership : uint8_t { NorthSouth = 0, EastWest = 1 };

constexpr std::array<Seat, 4> kAllSeats = { Seat::North, Seat::East, Seat::South, Seat::West };

constexpr Partnership partnershipOf(Seat seat) {
    return (seat == Seat::North || seat == Seat::South) ? Partnership::NorthSouth
                                                          : Partnership::EastWest;
}

constexpr Seat partnerOf(Seat seat) {
    switch (seat) {
        case Seat::North: return Seat::South;
        case Seat::South: return Seat::North;
        case Seat::East:  return Seat::West;
        case Seat::West:  return Seat::East;
    }
    return Seat::North; // unreachable; keeps -Wreturn-type quiet
}

// The next seat clockwise. Used for both bidding rotation and trick play.
constexpr Seat nextSeat(Seat seat) {
    return static_cast<Seat>((static_cast<uint8_t>(seat) + 1) % 4);
}

inline const char* seatName(Seat seat) {
    switch (seat) {
        case Seat::North: return "North";
        case Seat::East:  return "East";
        case Seat::South: return "South";
        case Seat::West:  return "West";
    }
    return "Unknown";
}

} // namespace bridge