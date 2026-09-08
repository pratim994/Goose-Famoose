#include "bridge/Hand.h"
#include <algorithm>

namespace bridge {

void Hand::addCard(const Card& card) {
    cards_.push_back(card);
}

bool Hand::removeCard(const Card& card) {
    const auto it = std::find(cards_.begin(), cards_.end(), card);
    if (it == cards_.end()) {
        return false;
    }
    cards_.erase(it);
    return true;
}

bool Hand::contains(const Card& card) const {
    return std::find(cards_.begin(), cards_.end(), card) != cards_.end();
}

bool Hand::hasSuit(Suit suit) const {
    return std::any_of(cards_.begin(), cards_.end(),
        [suit](const Card& c) { return c.suit == suit; });
}

std::vector<Card> Hand::cardsOfSuit(Suit suit) const {
    std::vector<Card> result;
    for (const auto& c : cards_) {
        if (c.suit == suit) {
            result.push_back(c);
        }
    }
    return result;
}

void Hand::sortForDisplay() {
    std::sort(cards_.begin(), cards_.end());
}

} // namespace bridge