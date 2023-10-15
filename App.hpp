#pragma once

#include "Renderer.hpp"
#include "GameObject.hpp"

namespace engine {

	class App {
	public:
		static constexpr int WIDTH = 1920;
		static constexpr int HEIGHT = 1200;

		App();
		~App();

		App(const App&) = delete;
		App& operator=(const App&) = delete;

		void run();

		void sierpinski(
			std::vector<Model::Vertex>& vertices,
			int depth,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 top);
	private:
		void loadGameObjects();

		Window m_window{ WIDTH, HEIGHT, "Hello Vulkan!" };
		Device m_device{ m_window };
		Renderer renderer{ m_window, m_device };
		std::vector<GameObject> m_gameObjects;
	};
} // namespace engine