#include "Logger.h"
#include "Window.h"
#include "plog/Init.h"

using namespace engine;

int main() {
	// Logger
	engine::Logger::init(plog::verbose);

	// Window
	std::string title = "Voxel Physics";
	Window* window = new Window(800, 800, title);

	// Loop
	float lastTimeSim = 0.0f;
	float lastTimeReal = 0.0f;
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	
	while (!window->shouldClose()) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTimeReal;

		window->pollEvents();
		window->swapBuffers();
		lastTimeReal = currentTime;
	}

	// Cleanup
	delete window;
	return 0;
}