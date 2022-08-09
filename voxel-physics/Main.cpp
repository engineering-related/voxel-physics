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
	Window* window = new Window(1600, 900, title, false);

	// GUI
	GUI* gui = new GUI(window);
	Shader* shader = new Shader("assets/shaders/vert.glsl",
                              "assets/shaders/frag.glsl");

	Voxel voxel1({ 0, 0, 0 }, { 0, 0, 0 }, { 0.5, 0, 0.59 });
	Voxel voxel2({ 1, 0, 0 }, { 0, 0, 0 }, { 0.698, 0.62, 0.12 });

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
			20.0f,          // Radius
			0.f,             // AzimuthAngle
			0.f,             // PolarAngle
			engine::perspective(FOV, aspectRatio, nearPlane, farPlane),
			FOV,
			aspectRatio,
			nearPlane,
			farPlane
		);
	}

	// Light
	vec3 lightDir(-1.0f, -1.0f, 0.0f);
	float ambient = 0.20f;
	float specular = 0.20f;

	// Loop
	float lastTimeSim = 0.0f;
	float lastTimeReal = 0.0f;
	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	uint32_t frameCounter = 0;

	while (!window->shouldClose()) {
		window->pollEvents();
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTimeReal;
		window->clear(BufferBit::COLOR | BufferBit::DEPTH);

		camera->update(deltaTime);

		shader->setUniform<mat4x4>("u_View", camera->getViewMatrix());
		shader->setUniform<mat4x4>("u_Projection", camera->getProjectionMatrix());
		shader->setUniform<vec3>("u_CamPos", camera->getEye());
		shader->setUniform<vec3>("u_LightDir", lightDir);
		shader->setUniform<float>("u_Ambient", ambient);
		shader->setUniform<float>("u_Specular", specular);

		voxel1.draw(shader, camera);
		voxel2.draw(shader, camera);

		voxel1.setRotation(vec3(voxel1.getRotation()) += deltaTime * 100);
		static float angle = 0;
		angle += deltaTime;
		voxel1.setPosition(vec3(cos(angle) + 2, sin(angle) + 2, sin(angle) + 2));

		gui->start();
		gui->submit([&]() {
			gui::Begin("Performance"); 
			gui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / gui::GetIO().Framerate, gui::GetIO().Framerate);
			gui::SliderFloat3("Light Direction", value_ptr(lightDir), -1.0f, 1.0f);
			gui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f);
			gui::SliderFloat("Specular", &specular, 0.0f, 1.0f);
			gui::End();
		});
		gui->end();

		window->swapBuffers();
		lastTimeReal = currentTime;
		frameCounter++;
	}

	// Cleanup
	delete window;
	delete gui;
	delete camera;
	delete shader;

	return 0;
}