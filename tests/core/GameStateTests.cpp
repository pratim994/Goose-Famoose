#include "core/GameState.hpp"

#include <cassert>

int main() {

    bridge::GameState state;

    assert(
        state.phase ==
        bridge::GamePhase::Lobby
    );

    assert(
        state.currentTrick.empty()
    );

    return 0;
}
