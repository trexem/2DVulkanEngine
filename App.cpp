#include "App.hpp"

#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>

namespace engine {

    App::App() {
        loadGameObjects();
    }

    App::~App() {}

	void App::run() {
        SimpleRenderSystem simpleRenderSystem(m_device, renderer.getSwapChainRenderPass());
        while (m_window.m_stillRunning) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {

                switch (event.type) {

                case SDL_QUIT:
                    m_window.m_stillRunning = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_q:
                        m_window.m_stillRunning = false;
                        break;

                    default:
                        //Do nothing
                        break;
                    }

                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED &&
                        event.window.windowID == SDL_GetWindowID(m_window.window)) {
                        m_window.framebufferResizeCallback();
                        m_device.recreateSurface();
                    }

                default:
                    // Do nothing.
                    break;
                }
            }

            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, m_gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_device.device());
	}

    void App::loadGameObjects() {
        std::vector<Model::Vertex> vertices{
            {{ -0.5f, 0.5f },{1.0f,0.0f,0.0f}},
            {{ 0.5f, 0.5f }, {0.0f,1.0f,0.0f}},
            {{ 0.0f, -0.5f },{0.0f,0.0f,1.0f}}
        };

        //sierpinski(vertices, 1, { -0.9f, 0.9f }, { 0.9f, 0.9f }, { 0.0f, -0.9f });
        auto model = std::make_shared<Model>(m_device, vertices);

        std::vector<glm::vec3> colors{
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f}
        };

        for (auto& color : colors) {
            color = glm::pow(color, glm::vec3{ 2.2f });
        }

        for (int i = 0; i < 40; i++) {
            auto triangle = GameObject::createGameObject();
            triangle.model = model;
            triangle.color = colors[i % colors.size()];
            triangle.transform2d.scale = glm::vec2(0.5f) + i * .005f;
            m_gameObjects.push_back(std::move(triangle));
        }
    }

    void App::sierpinski(
        std::vector<Model::Vertex>& vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({ top });
            vertices.push_back({ right });
            vertices.push_back({ left });
        }
        else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

} // namespace engine
