#include "bridge/Card.h"

namespace bridge {

std::string suitName(Suit suit) {
    switch (suit) {
        case Suit::Clubs:    return "Clubs";
        case Suit::Diamonds: return "Diamonds";
        case Suit::Hearts:   return "Hearts";
        case Suit::Spades:   return "Spades";
    }
    return "Unknown";
}

std::string suitSymbol(Suit suit) {
    switch (suit) {
        case Suit::Clubs:    return "\xE2\x99\xA3"; // ♣
        case Suit::Diamonds: return "\xE2\x99\xA6"; // ♦
        case Suit::Hearts:   return "\xE2\x99\xA5"; // ♥
        case Suit::Spades:   return "\xE2\x99\xA0"; // ♠
    }
    return "?";
}

std::string rankName(Rank rank) {
    switch (rank) {
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

std::string cardName(const Card& card) {
    return rankName(card.rank) + suitSymbol(card.suit);
}

} // namespace bridge