#include "pch.h"
#include "InterfaceManager.h"

#include "SceneManager.h"

/*
PURPOSE: Initialize interface manager
*/
INTERFACEMANAGER_API bool InterfaceManager::InitInterface(GLFWwindow* window, ImGuiContext*& context, ImNodesContext*& nodesContext)
{
	//initialize interface
	Logger::Log("P", "Initializing interface");

	this->window = window;

	//Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui::StyleColorsDark();

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
		return false;
	if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
		return false;

	//Save the current context to set context of other managers
	context = ImGui::GetCurrentContext();
	nodesContext = ImNodes::GetCurrentContext();

	return true;
}

/*
PURPOSE: Closes interface manager and releases other things
*/
void InterfaceManager::CloseInterface()
{
	//releases tabs
	ResetInterface();

	//releases imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
	glfwTerminate();

	//logger
	Logger::Log("P", "Released interface manager");
}

INTERFACEMANAGER_API void InterfaceManager::ResetInterface()
{
	openedTab = nullptr;
	tabs.clear();
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
	std::string& projectFilePath,
	std::function<void()> saveFunc,
	std::function<void()> closeFunc,
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
	std::vector<std::string> latestProjects,
	float engineFPS,
	float engineMS,
	int screenWidth,
	int screenHeight,
	bool projectOpened)
{
	//Draws menu bar
	MenuBar::GetInstance().DrawMenuBar(saveFunc, closeFunc, projectOpened);

	if (projectOpened)
		tabHeight = 40;
	else
		tabHeight = 0;

	if (projectOpened) {
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
					VisualScriptManager::GetInstance().currentScript = nullptr;
					//If the tabType is scene, set the currentScene
					if (tab->tabType == Utils::SceneEditor) {
						openedTab = tab;
					}
					//If the tabType is script, set the currentScript
					else if (tab->tabType == Utils::VisualScriptEditor) {
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
	}

	//Draws dock space
	DrawDockSpace();

	if (projectOpened) {
		//Draws windows
		if (openedTab) {
			if (openedTab->tabType == Utils::SceneEditor) {
				//If openedTabType is scene, draw scene windows
				if (SceneManager::GetInstance().openedScene && SceneManager::GetInstance().openedScene->GetEntityManager()) {
					if (WindowScene::GetInstance().showWindow)
						WindowScene::GetInstance().DrawWindow();

					if (WindowGameViewport::GetInstance().showWindow)
						WindowGameViewport::GetInstance().DrawWindow(engineFPS, engineMS);

					if (WindowEntityProperties::GetInstance().showWindow)
						WindowEntityProperties::GetInstance().DrawWindow(projectDir, tabs, openedTab, selectedTabId);

					if (WindowAssetExplorer::GetInstance().showWindow)
						WindowAssetExplorer::GetInstance().DrawWindow();

					if (WindowAddEntity::GetInstance().showWindow)
						WindowAddEntity::GetInstance().DrawWindow(entityTypes, projectDir, WindowScene::GetInstance().addParent);

					if (WindowAddAsset::GetInstance().showWindow)
						WindowAddAsset::GetInstance().DrawWindow(projectDir, WindowAssetExplorer::GetInstance().currentSelection);

					//TileMap windows
					if (WindowTileMapEdit::GetInstance().showWindow)
						WindowTileMapEdit::GetInstance().DrawWindow();

					if (WindowTileMapBrush::GetInstance().showWindow)
						WindowTileMapBrush::GetInstance().DrawWindow();

					if (WindowTileMapViewer::GetInstance().showWindow)
						WindowTileMapViewer::GetInstance().DrawWindow(screenWidth, screenHeight);

					//FlipBook windows
					if (WindowFlipBookEdit::GetInstance().showWindow)
						WindowFlipBookEdit::GetInstance().DrawWindow();
				}
			}
			else if (openedTab->tabType == Utils::VisualScriptEditor) {
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

		if (WindowAddScene::GetInstance().showWindow)
			WindowAddScene::GetInstance().DrawWindow(projectDir, projectFilePath, tabs, openedTab, selectedTabId);

		if (WindowProjectSettings::GetInstance().showWindow)
			WindowProjectSettings::GetInstance().DrawWindow();
	}
	else {
		WindowProjectDialog::GetInstance().DrawWindow(latestProjects);
	}

	if (WindowEditorSettings::GetInstance().showWindow)
		WindowEditorSettings::GetInstance().DrawWindow(&darkTheme, [this]() { SetDarkTheme(); });
	//std::bind(&InterfaceManager::SetDarkTheme, &InterfaceManager::GetInstance()) //TODO: I MIGHT USE THIS


	/* Modal Window */

	WindowModalDialog::GetInstance().OpenModalIfNeeded();

	WindowModalDialog::GetInstance().DrawWindow();
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
PURPOSE: On docking mode, imgui creates a new GLFWwindow object when a window is moved out from the main window
	So, we can't handle glfw events of the another window from main window's GLFWwindow.
	We have to get the GLFWwindow object of the focused window for now.
	This function is for getting the GLFWwindow object of the focused window.
*/
INTERFACEMANAGER_API GLFWwindow* InterfaceManager::GetFocusedViewport()
{
	//Get the focused viewport(window)
	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	ImGuiViewport* fallback = ImGui::GetMainViewport();

	for (ImGuiViewport* viewport : io.Viewports) {
		if (viewport->Flags & ImGuiViewportFlags_IsFocused) {
			if (viewport->PlatformHandleRaw) {
				//Get the GLFWwindow object 
				return static_cast<GLFWwindow*>(viewport->PlatformHandle);
			}
		}
	}

	// Fallback: main viewport's GLFWwindow
	if (fallback && fallback->PlatformHandleRaw) {
		return static_cast<GLFWwindow*>(fallback->PlatformHandle);
	}

	return nullptr;
}

/*
PURPOSE: Gets the instance of the class
*/
InterfaceManager& InterfaceManager::GetInstance()
{
	static InterfaceManager manager;
	return manager;
}
