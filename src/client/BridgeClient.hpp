
#pragma once

#include <memory>

#include "core/GameState.hpp"
#include "network/INetworkClient.hpp"

namespace bridge {
	
	class Renderer;
	class InputMananger;

	class BridgeClient {
		public:
			explicit BridgeClient (std::unique_ptr<Client> network);

			void start();

			void update(float deltaTime );

			void render();

			void recieveNetworkMessage(const Message& message);

			void mouseClick(float x, float y);

			const GameState& gameState() const;

		private:
			GameState gameState_;

			std::unique_ptr<INetworkClient> network_;

			std::unique_ptr<Renderer> renderer_;
			std::unique_ptr<InputManager> input_;

	};


}
