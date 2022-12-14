#include "Logger.h"
#include "Window.h"
#include "GUI.h"
#include "PointLight.h"
#include "Voxel.h"

using namespace engine;

int main() {
	// Logger
	engine::Logger::init(plog::verbose);

	// Window
	std::string title = "Voxel Physics";
	Window* window = new Window(1600, 900, title, false, false, true, 4);

	// GUI
	GUI* gui = new GUI(window);
	Shader* shader = new Shader("assets/shaders/vert.glsl",
                              "assets/shaders/frag.glsl");


	std::vector<Voxel*> voxels;
	{
		std::uniform_int_distribution<> densityDist(0, 1);
		std::uniform_real_distribution<> rotationDist(-90.f, 90.0f);
		std::uniform_real_distribution<> colorDist(0.f, 1.0f);
		std::uniform_real_distribution<> scaleDist(0.5f, 1.f);
		std::uniform_real_distribution<> forceDist(-1.f, 1.f);

		for (int32_t z = -10; z < 10; z++) {
			for (int32_t y = -10; y < 10; y++) {
				for (int32_t x = -10; x < 10; x++) {
					if (densityDist(rng) == 0) {
						vec3 position = { x, y, z };
						vec3 rotation = { rotationDist(rng), rotationDist(rng), rotationDist(rng) };
						vec3 scale = { scaleDist(rng), scaleDist(rng), scaleDist(rng) };
						vec3 color = { colorDist(rng), colorDist(rng), colorDist(rng) };
						Voxel* voxel = new Voxel(position, rotation, scale, color);
						vec3 force = { forceDist(rng), forceDist(rng), forceDist(rng) };
						voxel->applyForce(force);
						voxels.push_back(voxel);
					}
				}
			}
		}
	}

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
			150.0f,          // Radius
			0.f,             // AzimuthAngle
			0.f,             // PolarAngle
			perspective(FOV, aspectRatio, nearPlane, farPlane),
			FOV,
			aspectRatio,
			nearPlane,
			farPlane
		);
		// Change camera aspect ratio and projection matrix if window is resized
		window->addEventCallback(Event::WINDOW_RESIZED, [&]() {
			float aspectRatio = (float)window->getWidth() / (float)window->getHeight();
			camera->setApsectRatio(aspectRatio);
			camera->setProjectionMatrix(perspective(camera->getFOV(), aspectRatio, camera->getNearPlane(), camera->getFarPlane()));
		});
	}

	// Light
	PointLight* light;
	{
		vec3 direction = { -1.0f, -1.0f, -0.5f };
		float ambient = 0.20f;
		float specular = 0.40f;
		light = new PointLight(direction, ambient, specular);
	};

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

		for (auto& voxel : voxels)
			voxel->update(deltaTime);

		Voxel::drawAll(shader, camera, light);

		gui->start();
		gui->submit([&]() {
			gui::Begin("Controls                                                                       "); 
			gui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / gui::GetIO().Framerate, gui::GetIO().Framerate);
			gui::SliderFloat3("Light Direction", value_ptr(light->getDirection()), -1.0f, 1.0f);
			gui::SliderFloat("Ambient", &light->getAmbient(), 0.0f, 1.0f);
			gui::SliderFloat("Specular", &light->getSpecular(), 0.0f, 1.0f);
			gui::End();
		});
		gui->end();

		window->swapBuffers();
		lastTimeReal = currentTime;
		frameCounter++;
	}

	// Cleanup
	for (auto& voxel : voxels)
		delete voxel;
	voxels.clear();
	delete gui;
	delete camera;
	delete shader;
	delete light;
	delete window;

	return 0;
}