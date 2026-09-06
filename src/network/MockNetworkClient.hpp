#pragma once

#include "network/INetworkClient.hpp"

namespace bridge {

class MockNetworkClient final
    : public INetworkClient
{
public:
    void connect(
        const char* url
    ) override;

    void disconnect() override;

    void send(
        const Message& message
    ) override;

    bool isConnected() const override;

private:
    bool connected_{false};
};

} // namespace bridge