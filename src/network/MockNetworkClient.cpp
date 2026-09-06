#include "network/MockNetworkClient.hpp"

#include <iostream>

namespace bridge {

void MockNetworkClient::connect(
    const char*
) {
    connected_ = true;

    std::cout << "Mock connection established\n";
}

void MockNetworkClient::disconnect() {
    connected_ = false;
}

void MockNetworkClient::send(
    const Message& message
) {
    std::cout
        << "Mock send: "
        << static_cast<int>(message.type)
        << '\n';
}

bool MockNetworkClient::isConnected() const {
    return connected_;
}

} // namespace bridge