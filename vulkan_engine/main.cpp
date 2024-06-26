#include "tve_app.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
	tve::EngineApp app{};

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}