
#pragma once 
#include "network/InetworkClient.hpp"


namespace bridge {

	class WasmNetworkClient final : public InetworkClient {
		public :
			void connect(const char* url) override;

			void disconnect() override;

			void send(const Message& message) override;

			bool isConnected() const override;

			void onOpen();

			void onMessage(const char* data);

			void onClose();

		private:
			bool connected_{false};
	};

}
