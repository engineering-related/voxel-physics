#include "Logger.h"
#include "Window.h"
#include "FrameBuffer.h"
#include "GUI.h"

using namespace engine;

// TODO: This is very unoptimized but easy to use/understand
struct Voxel {
	vec3 rotation;
	uvec4 color;
};

struct VoxelVolume {
	uvec3 size;
};

int main() {
	// Logger
	engine::Logger::init(plog::verbose);

	// Window
	std::string title = "Voxel Physics";
	Window* window = new Window(800, 800, title);
	GUI* gui = new GUI(window);

	// Loop
	float lastTimeSim = 0.0f;
	float lastTimeReal = 0.0f;
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	
	while (!window->shouldClose()) {
		window->pollEvents();
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTimeReal;
		window->clear(BufferBit::COLOR | BufferBit::DEPTH);

		gui->start();
		gui->submit([]() {
			ImGui::Begin("Performance"); 
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		});
		gui->end();

		window->swapBuffers();
		lastTimeReal = currentTime;
	}

	// Cleanup
	delete window;
	delete gui;

	return 0;
}