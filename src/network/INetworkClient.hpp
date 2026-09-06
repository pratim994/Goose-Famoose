#pragma once

#include "protocol/Message.hpp"

#include <functional>

namespace bridge {

class INetworkClient {
public:
    using MessageHandler =
        std::function<void(const Message&)>;

    virtual ~INetworkClient() = default;

    virtual void connect(
        const char* url
    ) = 0;

    virtual void disconnect() = 0;

    virtual void send(
        const Message& message
    ) = 0;

    virtual bool isConnected() const = 0;

    void setMessageHandler(
        MessageHandler handler
    ) {
        messageHandler_ = std::move(handler);
    }

protected:
    void receive(const Message& message) {
        if (messageHandler_) {
            messageHandler_(message);
        }
    }

private:
    MessageHandler messageHandler_;
};

} // namespace bridge