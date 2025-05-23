#include "pch.h"
#include "InterfaceManager.h"

#include "SceneManager.h"

/*
PURPOSE: Initialize interface manager
*/
bool InterfaceManager::InitInterface(GLFWwindow* window)
{
	/*
	Initialize glfw
	FIXME: in every dll, i don't want to initialize this
		so this initialization will be removed.
	*/
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed in DLL!" << std::endl;
		return false;
	}

	/*
	Loads glad functions
	FIXME: and this one
	*/
	int version = gladLoadGL();
	if (version == 0) {
		Logger::Log("E", "Starting engine failed in gladLoadGLLoader");
		return false;
	}

	//initialize interface
	Logger::Log("P", "Initializing interface");

	//Initialize imgui
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

/*
PURPOSE: Closes interface manager and releases other things
*/
void InterfaceManager::CloseInterface()
{
	//releases tabs
	openedTab = nullptr;
	tabs.clear();
	//releases imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
	glfwTerminate();
	//logger
	Logger::Log("P", "Released interface manager");
}

/*
PURPOSE: In every frame create a new imgui frame
*/
void InterfaceManager::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

/*
PURPOSE: Render imgui
*/
void InterfaceManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
PURPOSE: Draws every imgui window, also draws gameviewport as a image from framebuffertexture
*/
void InterfaceManager::DrawInterface(
	std::string& projectDir,
	std::function<void()> saveFunc,
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
	float engineFPS,
	float engineMS)
{
	//Draws menu bar
	MenuBar::GetInstance().DrawMenuBar(saveFunc);

	//Some variables
	int windowHeight = (int)ImGui::GetWindowHeight();
	int tabWidth = (int)ImGui::GetContentRegionAvail().x;
	int titleHeight = (int)ImGui::GetContentRegionAvail().y;

	//Begins tabbar
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, (float)tabHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Tabs", nullptr, window_flags);
	ImGui::SetWindowFontScale(1.5f);
	ImGui::PopStyleVar(3);

	//Draw each tab
	for (auto& tabIter : InterfaceManager::GetInstance().tabs) {
		//Get if the drawing tab is selected
		bool selected = false;
		auto tab = tabIter.second.get();
		if (openedTab) {
			selected = tab->id == openedTab->id;
		}
		if (tab && tab->id.c_str()) {
			if (ImGui::Selectable(tab->id.c_str(), selected, ImGuiSelectableFlags_None, ImVec2(100, (float)tabHeight))) {
				SceneManager::GetInstance().currentScene = nullptr;
				VisualScriptManager::GetInstance().currentScript = nullptr;
				//If the tabType is scene, set the currentScene
				if (tab->tabType == SceneEditor) {
					auto openedScene = SceneManager::GetInstance().openedScenes.find(tab->id);
					if (openedScene != SceneManager::GetInstance().openedScenes.end()) {
						SceneManager::GetInstance().currentScene = openedScene->second.get();
						openedTab = tab;
					}
				}
				//If the tabType is script, set the currentScript
				else if (tab->tabType == VisualScriptEditor) {
					auto openedScript = VisualScriptManager::GetInstance().openedScripts.find(tab->id);
					if (openedScript != VisualScriptManager::GetInstance().openedScripts.end()) {
						VisualScriptManager::GetInstance().currentScript = openedScript->second;
						openedTab = tab;
					}
				}
			}

			//Deleting tab
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				deleteTabId = tab->id;
				pendingTabDelete = true;
			}
		}
		ImGui::SameLine();
	}

	ImGui::End();

	//Draws dock space
	DrawDockSpace();

	//Draws windows
	if (openedTab) {
		if (openedTab->tabType == SceneEditor) {
			//If openedTabType is scene, draw scene windows
			if (SceneManager::GetInstance().currentScene && SceneManager::GetInstance().currentScene->GetEntityManager()) {
				if (WindowScene::GetInstance().showWindow)
					WindowScene::GetInstance().DrawWindow();

				if (WindowGameViewport::GetInstance().showWindow)
					WindowGameViewport::GetInstance().DrawWindow(engineFPS, engineMS);

				if (WindowEntityProperties::GetInstance().showWindow)
					WindowEntityProperties::GetInstance().DrawWindow(projectDir, tabs, openedTab, selectedTabId);

				if (WindowAssetExplorer::GetInstance().showWindow)
					WindowAssetExplorer::GetInstance().DrawWindow();
			}
		}
		else if (openedTab->tabType == VisualScriptEditor) {
			//If openedTabType is script, draw script windows
			if (VisualScriptManager::GetInstance().currentScript) {
				if (WindowVisualScript::GetInstance().showWindow) {
					WindowVisualScript::GetInstance().DrawWindow(tabHeight, projectDir, openedTab, tabs);
				}
			}
		}
	}

	if (WindowAllScenes::GetInstance().showWindow)
		WindowAllScenes::GetInstance().DrawWindow(projectDir, entityTypes, WindowAddScene::GetInstance().showWindow, WindowEntityProperties::GetInstance().currentEntity, WindowScene::GetInstance().selectedId, tabs, selectedTabId, openedTab);

	if (WindowEditorSettings::GetInstance().showWindow)
		WindowEditorSettings::GetInstance().DrawWindow(&darkTheme, [this]() { SetDarkTheme(); });
		//std::bind(&InterfaceManager::SetDarkTheme, &InterfaceManager::GetInstance()) //TODO: I MIGHT USE THIS

	if (WindowAddEntity::GetInstance().showWindow)
		WindowAddEntity::GetInstance().DrawWindow(entityTypes, projectDir, WindowScene::GetInstance().addParent);

	if (WindowAddAsset::GetInstance().showWindow)
		WindowAddAsset::GetInstance().DrawWindow(projectDir, WindowAssetExplorer::GetInstance().currentSelection);

	if (WindowAddScene::GetInstance().showWindow)
		WindowAddScene::GetInstance().DrawWindow(projectDir, tabs, openedTab, selectedTabId);

	if (WindowProjectSettings::GetInstance().showWindow)
		WindowProjectSettings::GetInstance().DrawWindow();
}

/*
PURPOSE: Sets the parameters and draws dock space
*/
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

/*
PURPOSE: Updates imgui windows when they are showing as another windows,
	this function allows us to separate imgui windows out of main window
*/
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

/*
PURPOSE: Sets the imgui theme to the selected one
*/
void InterfaceManager::SetDarkTheme()
{
	if (darkTheme)
		ImGui::StyleColorsDark();
	else
		ImGui::StyleColorsLight();
}

/*
PURPOSE: Gets the instance of the class
*/
InterfaceManager& InterfaceManager::GetInstance()
{
	static InterfaceManager manager;
	return manager;
}
