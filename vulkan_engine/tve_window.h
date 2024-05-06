#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace tve {

	class TveWindow {

	public: 
		TveWindow(int w, int h, std::string name);
		~TveWindow();

		TveWindow(const TveWindow&) = delete;
		TveWindow &operator = (const TveWindow&) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool shouldClose() { return glfwWindowShouldClose(window);};

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		GLFWwindow *window;

		const int width, height;
		std::string windowName;

		void initWindow();
	};
}