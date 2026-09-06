#pragma once

#include "protocol/Message.hpp"

#include <string>

namespace bridge {

class Protocol {
public:
    static std::string encode(
        const Message& message
    );

    static Message decode(
        const std::string& data
    );
};

} // namespace bridge