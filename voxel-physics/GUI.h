#ifndef IM_GUI
#define IM_GUI

#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace engine 
{
	namespace gui = ImGui;

	class GUI
	{
	public:
		GUI(Window* window) {
			setup(window);
		}

		~GUI() {
			cleanup();
		}

		void submit(const std::function<void()>& function) 
		{
			m_RenderFunctions.push_back(function);
		}

		void start() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			gui::NewFrame();
		}

		void end() {
			for (auto& f : m_RenderFunctions) 
				f();
			gui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(gui::GetDrawData());
			m_RenderFunctions.clear();
		}

	private:
		std::vector<std::function<void()>> m_RenderFunctions;

		void setup(Window* window) {
			gui::CreateContext();
			ImGuiIO& io = gui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			gui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(window->getGlfwWindowPtr(), true);
			ImGui_ImplOpenGL3_Init("#version 460");
		}

		void cleanup() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			gui::DestroyContext();
		}

	};
}

#endif // !IM_GUI
