#include "InterfaceManager.h"

bool InterfaceManager::InitInterface(GLFWwindow* window)
{
	//initialize interface
	Logger::Log("P", "Initializing interface");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	return true;
}

void InterfaceManager::CloseInterface()
{
	//closing
	openedTab = nullptr;
	tabs.clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImNodes::DestroyContext();
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

	int windowHeight = ImGui::GetWindowHeight();
	int tabWidth = ImGui::GetContentRegionAvail().x;
	int titleHeight = ImGui::GetContentRegionAvail().y;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, tabHeight));
	////ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Tabs", nullptr, window_flags);
	ImGui::SetWindowFontScale(1.5f);

	ImGui::PopStyleVar(3);

	for (auto& tabIter : InterfaceManager::GetInstance().tabs) {
		bool selected = false;
		auto tab = tabIter.second.get();
		if (openedTab) {
			selected = tab->id == openedTab->id;
		}
		if (ImGui::Selectable(tab->id.c_str(), selected, ImGuiSelectableFlags_None, ImVec2(100, tabHeight))) {
			SceneManager::GetInstance().currentScene = nullptr;
			VisualScriptManager::GetInstance().currentScript = nullptr;
			if (tab->tabType == SceneEditor) {
				auto openedScene = SceneManager::GetInstance().openedScenes.find(tab->id);
				if (openedScene != SceneManager::GetInstance().openedScenes.end()) {
					SceneManager::GetInstance().currentScene = openedScene->second.get();
					openedTab = tab;
				}
			}
			else if(tab->tabType == VisualScriptEditor) {
				auto openedScript = VisualScriptManager::GetInstance().openedScripts.find(tab->id);
				if (openedScript != VisualScriptManager::GetInstance().openedScripts.end()) {
					VisualScriptManager::GetInstance().currentScript = openedScript->second;
					openedTab = tab;
				}
			}
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
			deleteTabId = tab->id;
			pendingTabDelete = true;
		}
		ImGui::SameLine();
	}

	ImGui::End();

	DrawDockSpace();

	if (openedTab) {
		if (openedTab->tabType == SceneEditor) {
			if (SceneManager::GetInstance().currentScene && SceneManager::GetInstance().currentScene->GetEntityManager()) {
				if (WindowScene::GetInstance().showWindow)
					WindowScene::GetInstance().DrawWindow();

				if (WindowGameViewport::GetInstance().showWindow)
					WindowGameViewport::GetInstance().DrawWindow();

				if (WindowEntityProperties::GetInstance().showWindow)
					WindowEntityProperties::GetInstance().DrawWindow();
			}
		}
		else if (openedTab->tabType == VisualScriptEditor) {
			if (VisualScriptManager::GetInstance().currentScript) {
				if (WindowVisualScript::GetInstance().showWindow)
					WindowVisualScript::GetInstance().DrawWindow();
			}
		}
	}
	if (WindowAllScenes::GetInstance().showWindow)
		WindowAllScenes::GetInstance().DrawWindow();

	if (WindowEditorSettings::GetInstance().showWindow)
		WindowEditorSettings::GetInstance().DrawWindow();

	if (WindowAddEntity::GetInstance().showWindow)
		WindowAddEntity::GetInstance().DrawWindow();

	if (WindowProjectSettings::GetInstance().showWindow)
		WindowProjectSettings::GetInstance().DrawWindow();

	
}

void InterfaceManager::DrawDockSpace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + tabHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - tabHeight));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Main", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2(0, 0));
	ImGui::End();
}

void InterfaceManager::UpdateViewportContext()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void InterfaceManager::SetDarkTheme()
{
	if (darkTheme)
		ImGui::StyleColorsDark();
	else
		ImGui::StyleColorsLight();
}

InterfaceManager& InterfaceManager::GetInstance()
{
	static InterfaceManager manager;
	return manager;
}
