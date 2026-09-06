#include "client/BridgeClient.hpp"

#include "input/InputManager.hpp"
#include "rendering/Renderer.hpp"

namespace bridge {

BridgeClient::BridgeClient(
    std::unique_ptr<INetworkClient> network
)
    : network_(std::move(network)),
      renderer_(std::make_unique<Renderer>()),
      input_(std::make_unique<InputManager>())
{
    network_->setMessageHandler(
        [this](const Message& message) {
            receiveNetworkMessage(message);
        }
    );
}

void BridgeClient::start() {
    network_->connect(
        "wss://example.invalid/game"
    );
}

void BridgeClient::update(
    float deltaTime
) {
    input_->update(deltaTime);
}

void BridgeClient::render() {
    renderer_->render(gameState_);
}

void BridgeClient::receiveNetworkMessage(
    const Message&
) {
}

void BridgeClient::mouseClick(
    float x,
    float y
) {
    input_->mouseClick(x, y);

}

const GameState& BridgeClient::gameState() const {
    return gameState_;
}
}
