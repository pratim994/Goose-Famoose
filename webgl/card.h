#pragma once
#include <cstdint>
#include <string>

// ============================================================
// Core Bridge data model. Pure data — no OpenGL here whatsoever.
// ============================================================

enum class Suit : uint8_t {
    Clubs = 0,
    Diamonds,
    Hearts,
    Spades
};

enum class Rank : uint8_t {
    Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Ace
};

enum class Seat : uint8_t {
    North = 0, East, South, West
};

struct Card {
    Suit suit;
    Rank rank;

    bool operator==(const Card& o) const { return suit == o.suit && rank == o.rank; }
};

// Suit and rank are known at every seat regardless of language, so keep
// these tiny helpers around for debug logging / window titles only.
// (Per project rules these are NOT the primary deliverable — the card
// must be *drawn*, this text is just useful for a console log line.)
inline const char* suitName(Suit s) {
    switch (s) {
        case Suit::Clubs:    return "Clubs";
        case Suit::Diamonds: return "Diamonds";
        case Suit::Hearts:   return "Hearts";
        case Suit::Spades:   return "Spades";
    }
    return "?";
}

inline const char* rankShortName(Rank r) {
    switch (r) {
        case Rank::Two:   return "2";
        case Rank::Three: return "3";
        case Rank::Four:  return "4";
        case Rank::Five:  return "5";
        case Rank::Six:   return "6";
        case Rank::Seven: return "7";
        case Rank::Eight: return "8";
        case Rank::Nine:  return "9";
        case Rank::Ten:   return "10";
        case Rank::Jack:  return "J";
        case Rank::Queen: return "Q";
        case Rank::King:  return "K";
        case Rank::Ace:   return "A";
    }
    return "?";
}

inline const char* seatName(Seat s) {
    switch (s) {
        case Seat::North: return "North";
        case Seat::East:  return "East";
        case Seat::South: return "South";
        case Seat::West:  return "West";
    }
    return "?";
}

// True for Hearts/Diamonds — used purely to pick red vs black ink color.
inline bool isRedSuit(Suit s) { return s == Suit::Hearts || s == Suit::Diamonds; }

// Numeric pip count used for laying out center pips on 2..10 cards (Ace
// is handled as a single big pip, face cards get a letter glyph instead).
inline int pipCount(Rank r) { return static_cast<int>(r) + 2; }