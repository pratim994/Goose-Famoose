
#include <iostream>
#include "rendering/rendering.hpp"


namespace bridge {

	void Renderer::render(const GameState& state); {
		std::cout << "Rendering phase:" << static_cast<int>(state.phase) << '\n';
	}
}
