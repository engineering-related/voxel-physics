#include "Logger.h"
#include "Window.h"
#include "GUI.h"
#include "Voxel.h"

using namespace engine;

int main() {
	// Logger
	engine::Logger::init(plog::verbose);

	// Window
	std::string title = "Voxel Physics";
	Window* window = new Window(800, 800, title);

	// GUI
	GUI* gui = new GUI(window);
	Shader* shader = new Shader("assets/shaders/vert.glsl",
                              "assets/shaders/frag.glsl");

	Voxel voxel1({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	Voxel voxel2({ 1, 0, 0 }, { 0, 0, 0 }, { 1, 0, 1 });

	// Camera
	Camera* camera;
	{
		float FOV = 45.0f;
		float aspectRatio = (float)window->getWidth() / (float)window->getHeight();
		float nearPlane = 0.1f;
		float farPlane = 2500.0f;

		camera = new Camera (
			window,
			{0.f, 0.f, 0.f}, // Center
			{0, 1, 0},       // UpVector
			50.0f,          // Radius
			0.f,             // AzimuthAngle
			0.f,             // PolarAngle
			engine::perspective(FOV, aspectRatio, nearPlane, farPlane),
			FOV,
			aspectRatio,
			nearPlane,
			farPlane
		);
	}

	// Render simple test cube
  
	float lastTimeSim = 0.0f;
	float lastTimeReal = 0.0f;
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	
	while (!window->shouldClose()) {
		window->pollEvents();
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTimeReal;
		window->clear(BufferBit::COLOR | BufferBit::DEPTH);

		camera->update(deltaTime);

		voxel1.draw(shader, camera);
		voxel2.draw(shader, camera);

		gui->start();
		gui->submit([]() {
			gui::Begin("Performance"); 
			gui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / gui::GetIO().Framerate, gui::GetIO().Framerate);
			gui::End();
		});
		gui->end();

		window->swapBuffers();
		lastTimeReal = currentTime;
	}

	// Cleanup
	delete window;
	delete gui;
	delete camera;
	delete shader;

	return 0;
}