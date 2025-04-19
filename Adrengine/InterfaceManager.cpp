#include "InterfaceManager.h"

bool InterfaceManager::InitInterface(GLFWwindow* window)
{
	//initialize interface
	Logger::Log("P", "Initializing interface");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
 
	return true;
}

void InterfaceManager::CloseInterface()
{
	//closing
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Logger::Log("P", "Released interface");
}

void InterfaceManager::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void InterfaceManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InterfaceManager::DrawInterface()
{
	MenuBar::GetInstance().DrawMenuBar();
	
	if(WindowProjectSettings::GetInstance().showWindow)
		WindowProjectSettings::GetInstance().DrawWindow();

}

InterfaceManager& InterfaceManager::GetInstance()
{
	static InterfaceManager manager;
	return manager;
}
