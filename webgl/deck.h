#pragma once
#include <array>
#include <vector>
#include <random>
#include "Card.h"

// Owns the 52 cards, can shuffle and deal four 13-card hands.
// Contains zero rendering / OpenGL knowledge.
class Deck {
public:
    Deck();

    void reset();                 // rebuild the ordered 52-card deck
    void shuffle(unsigned seed);  // Fisher-Yates shuffle
    void shuffle();               // shuffle with a random_device seed

    // Deals all 52 cards, 13 to each seat, in North/East/South/West order.
    std::array<std::vector<Card>, 4> dealAll() const;

    const std::vector<Card>& cards() const { return cards_; }
    size_t size() const { return cards_.size(); }

private:
    std::vector<Card> cards_;
};