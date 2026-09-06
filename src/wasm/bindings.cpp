#include <emscripten/bind.h>

#include "client/BridgeClient.hpp"
#include "network/WasmNetworkClient.hpp"

#include <memory>

namespace {

std::unique_ptr<bridge::BridgeClient> client;

}

void initialize() {

    auto network =
        std::make_unique<
            bridge::WasmNetworkClient
        >();

    client =
        std::make_unique<
            bridge::BridgeClient
        >(
            std::move(network)
        );

    client->start();
}

void update(
    float deltaTime
) {
    if (client) {
        client->update(deltaTime);
    }
}

void render() {
    if (client) {
        client->render();
    }
}

void mouseClick(
    float x,
    float y
) {
    if (client) {
        client->mouseClick(x, y);
    }
}

EMSCRIPTEN_BINDINGS(bridge_client) {

    emscripten::function(
        "initialize",
        &initialize
    );

    emscripten::function(
        "update",
        &update
    );

    emscripten::function(
        "render",
        &render
    );

    emscripten::function(
        "mouseClick",
        &mouseClick
    );
}
