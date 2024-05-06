#include "tve_app.h"

// std
#include <array>
#include <stdexcept>

namespace tve {

    EngineApp::EngineApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    EngineApp::~EngineApp() { vkDestroyPipelineLayout(tveDevice.device(), pipelineLayout, nullptr); }

    void EngineApp::run() {
        while (!tveWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(tveDevice.device());
    }

    void EngineApp::loadModels() {
        std::vector<TveModel::Vertex> vertices{{{0.0f, -0.5f}}, { {0.5f, 0.5f} }, { {-0.5f, 0.5f} }};
        tveModel = std::make_unique<TveModel>(tveDevice, vertices);
    }

    void EngineApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(tveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void EngineApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};
        TvePipeline::defaultPipelineConfigInfo(
            pipelineConfig,
            tveSwapChain.width(),
            tveSwapChain.height());
        pipelineConfig.renderPass = tveSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        tvePipeline = std::make_unique<TvePipeline>(
            tveDevice,
            "simple_shader.vert.spv",
            "simple_shader.frag.spv",
            pipelineConfig);
    }

    void EngineApp::createCommandBuffers() {
        commandBuffers.resize(tveSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = tveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(tveDevice.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = tveSwapChain.getRenderPass();
            renderPassInfo.framebuffer = tveSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = tveSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            tvePipeline->bind(commandBuffers[i]);
            tveModel->bind(commandBuffers[i]);
            tveModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }
    void EngineApp::drawFrame() {
        uint32_t imageIndex;
        auto result = tveSwapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = tveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

}  // namespace tve