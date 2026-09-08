#pragma once
#include <cstddef>
#include <vector>
#include "bridge/Card.h"

namespace bridge {

// Hand is a simple, ordered collection of the cards a player currently
// holds. It knows nothing about turns, other players, or legality of play --
// those rules live in higher-level game logic (Auction/Trick, later phases).
class Hand {
public:
    void addCard(const Card& card);

    // Removes the given card if present. Returns true if it was removed,
    // false if the card was not in the hand.
    bool removeCard(const Card& card);

    bool contains(const Card& card) const;
    bool hasSuit(Suit suit) const;

    std::vector<Card> cardsOfSuit(Suit suit) const;

    const std::vector<Card>& cards() const { return cards_; }
    std::size_t size() const { return cards_.size(); }
    bool isEmpty() const { return cards_.empty(); }

    void clear() { cards_.clear(); }

    // Sorts by suit then rank. Purely a display convenience (e.g. showing a
    // human-readable hand) -- gameplay logic never depends on hand order.
    void sortForDisplay();

private:
    std::vector<Card> cards_;
};

} // namespace bridge