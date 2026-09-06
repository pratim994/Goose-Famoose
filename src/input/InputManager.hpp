#pragma once

namespace bridge {

class InputManager {
public:
    void update(float deltaTime);

    void mouseClick(
        float x,
        float y
    );
};

} // namespace bridge
