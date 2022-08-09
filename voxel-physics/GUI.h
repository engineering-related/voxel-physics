#ifndef IM_GUI
#define IM_GUI

#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace engine 
{
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
			ImGui::NewFrame();
		}

		void end() {
			for (auto& f : m_RenderFunctions) 
				f();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			m_RenderFunctions.clear();
		}

	private:
		std::vector<std::function<void()>> m_RenderFunctions;

		void setup(Window* window) {
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(window->getGlfwWindowPtr(), true);
			ImGui_ImplOpenGL3_Init("#version 460");
		}

		void cleanup() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

	};
}

#endif // !IM_GUI
