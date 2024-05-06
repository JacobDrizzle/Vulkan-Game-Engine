#pragma once

#include "tve_window.h"
#include "tve_pipeline.h"
#include "tve_device.h"
#include "tve_swap_chain.h"
#include "tve_model.h"


// std
#include <memory>
#include <vector>


namespace tve {
	class EngineApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		EngineApp();
		~EngineApp();

		EngineApp(const EngineApp&) = delete;
		EngineApp& operator=(const EngineApp&) = delete;

		void run();

	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		TveWindow tveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
		TveDevice tveDevice{tveWindow};
		TveSwapChain tveSwapChain{ tveDevice, tveWindow.getExtent() };
		std::unique_ptr<TvePipeline> tvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<TveModel> tveModel;
	};
}