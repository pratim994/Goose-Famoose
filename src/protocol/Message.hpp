#pragma once

#include <cstdint>
#include <string>

namespace bridge {

enum class MessageType : std::uint8_t {
    JoinTable,
    LeaveTable,

    TableState,

    PlayerJoined,
    PlayerLeft,

    MakeBid,
    BidAccepted,
    BidRejected,

    PlayCard,
    CardPlayed,

    TrickCompleted,

    HandCompleted,

    Error
};

struct Message {
    MessageType type{};
    std::string payload;
};

} // namespace bridge