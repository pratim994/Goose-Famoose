#include "Deck.h"
#include <algorithm>

Deck::Deck() {
    reset();
}

void Deck::reset() {
    cards_.clear();
    cards_.reserve(52);
    for (int s = 0; s < 4; ++s) {
        for (int r = 0; r < 13; ++r) {
            cards_.push_back(Card{ static_cast<Suit>(s), static_cast<Rank>(r) });
        }
    }
}

void Deck::shuffle(unsigned seed) {
    std::mt19937 rng(seed);
    std::shuffle(cards_.begin(), cards_.end(), rng);
}

void Deck::shuffle() {
    std::random_device rd;
    shuffle(rd());
}

std::array<std::vector<Card>, 4> Deck::dealAll() const {
    std::array<std::vector<Card>, 4> hands;
    // cards_ must have 52 entries for a clean deal; deal round-robin
    // starting at North, which is the standard Bridge dealing order.
    for (size_t i = 0; i < cards_.size(); ++i) {
        hands[i % 4].push_back(cards_[i]);
    }
    for (auto& hand : hands) {
        // Sort each hand for a readable fan: Spades, Hearts, Diamonds, Clubs
        // high-to-low, which is the conventional Bridge hand display order.
        std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
            // Suit order for display: Spades(3) > Hearts(2) > Diamonds(1) > Clubs(0)
            if (a.suit != b.suit) return static_cast<int>(a.suit) > static_cast<int>(b.suit);
            return static_cast<int>(a.rank) > static_cast<int>(b.rank);
        });
    }
    return hands;
}