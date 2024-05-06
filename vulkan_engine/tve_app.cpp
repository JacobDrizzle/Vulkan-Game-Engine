#include "tve_app.h"

namespace tve {

	void EngineApp::run() {
		while (!tveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}

}