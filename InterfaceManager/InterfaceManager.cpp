#include "pch.h"
#include "InterfaceManager.h"

#include "MenuBar.h"

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
	WindowEntityProperties::GetInstance().SelectEntity(nullptr);
	WindowScene::GetInstance().selectedId = "";
	WindowScene::GetInstance().addParent = nullptr;


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
void InterfaceManager::DrawInterface()
{
	//Draws menu bar
	MenuBar::GetInstance().DrawMenuBar();

	//Project instance from service locator
	auto projectInterface = ServiceLocator::Get<IProject>();

	//Update tabHeight
	if (projectInterface->GetProjectOpened())
		tabHeight = 40;
	else
		tabHeight = 0;
	
	//Draw tabbar if a project is opened
	if (projectInterface->GetProjectOpened()) {
		DrawTabbar();
	}

	//Draws dock space
	DrawDockSpace();

	//Draw all windows
	DrawWindows();
}

/*
PURPOSE: Add a new tab to the tabs bar, and activates it
*/
INTERFACEMANAGER_API void InterfaceManager::AddTab(std::string tabId, Utils::TabType tabType)
{
	//Create a tab and insert it to tabs
	std::shared_ptr<Utils::Tab> tab = std::make_shared<Utils::Tab>();
	tab->id = tabId;
	tab->tabType = tabType;
	tabs.insert(std::pair<std::string, std::shared_ptr<Utils::Tab>>(tab->id, tab));
}

/*
PURPOSE: Activates a tab by its id
*/
INTERFACEMANAGER_API void InterfaceManager::ActivateTab(std::string tabId)
{
	//Find the tab
	auto tab = GetTabById(tabId);

	//Set the openedTab and selectedTabId
	if (tab) {
		//If a tab found, set it as openedTab
		openedTab = tab;
		selectedTabId = tab->id;
	}
	else {
		//If no tab found, set openedTab to nullptr
		openedTab = nullptr;
		selectedTabId = "";
	}
}

/*
PURPOSE: Removes a tab by its id
*/
INTERFACEMANAGER_API void InterfaceManager::RemoveTab(std::string tabId)
{
	//Find the tab
	auto tabIter = tabs.find(tabId);
	if (tabIter == tabs.end())
		return;

	//Delete tab
	tabs.erase(tabIter);

	//If the removed tab is the openedTab, set openedTab to nullptr
	if (openedTab && openedTab->id == tabId) {
		openedTab = nullptr;
		selectedTabId = "";
	}
}

/*
PURPOSE: Removes all tabs
*/
INTERFACEMANAGER_API void InterfaceManager::RemoveAllTabs()
{
	openedTab = nullptr;
	selectedTabId = "";
	tabs.clear();
}

/*
PURPOSE: Returns a tab by its id
*/
INTERFACEMANAGER_API std::shared_ptr<Utils::Tab> InterfaceManager::GetTabById(std::string tabId)
{
	//Find the tab
	auto tabIter = tabs.find(tabId);
	if (tabIter == tabs.end())
		return nullptr;

	//Return the tab
	return tabIter->second;
}

/*
PURPOSE: Returns the tab that is going to be deleted
*/
INTERFACEMANAGER_API std::shared_ptr<Utils::Tab> InterfaceManager::GetDeletingTab()
{
	//Find the tab
	auto tab = GetTabById(deleteTabId);
	if (!tab)
		return nullptr;

	//Return the tab
	return tab;
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
PURPOSE: Draws the tabbar that will be used to handle scene and visual script pages
*/
void InterfaceManager::DrawTabbar()
{
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
		auto tab = tabIter.second;
		if (openedTab) {
			selected = tab->id == openedTab->id;
		}
		if (tab && tab->id.c_str()) {
			if (ImGui::Selectable(tab->id.c_str(), selected, ImGuiSelectableFlags_None, ImVec2(100, (float)tabHeight))) {
				ServiceLocator::Get<IVisualScriptManager>()->SetCurrentScript(nullptr);
				//If the tabType is scene, set the currentScene
				if (tab->tabType == Utils::SceneEditor) {
					openedTab = tab;
				}
				//If the tabType is script, set the currentScript
				else if (tab->tabType == Utils::VisualScriptEditor) {
					auto openedScript = ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts().find(tab->id);
					if (openedScript != ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts().end()) {
						ServiceLocator::Get<IVisualScriptManager>()->SetCurrentScript(openedScript->second);
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

/*
PURPOSE: Draws all imgui windows
*/
void InterfaceManager::DrawWindows()
{
	if (ServiceLocator::Get<IProject>()->GetProjectOpened()) {
		//Draws windows
		if (openedTab) {
			if (openedTab->tabType == Utils::SceneEditor) {
				//If openedTabType is scene, draw scene windows
				if (ServiceLocator::Get<ISceneManager>()->GetOpenedScene() && ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()) {
					if (WindowScene::GetInstance().showWindow)
						WindowScene::GetInstance().DrawWindow();

					if (WindowGameViewport::GetInstance().showWindow)
						WindowGameViewport::GetInstance().DrawWindow();

					if (WindowEntityProperties::GetInstance().showWindow)
						WindowEntityProperties::GetInstance().DrawWindow();

					if (WindowAssetExplorer::GetInstance().showWindow)
						WindowAssetExplorer::GetInstance().DrawWindow();

					if (WindowAddEntity::GetInstance().showWindow)
						WindowAddEntity::GetInstance().DrawWindow();

					if (WindowAddAsset::GetInstance().showWindow)
						WindowAddAsset::GetInstance().DrawWindow(WindowAssetExplorer::GetInstance().currentSelection);

					//TileMap windows
					if (WindowTileMapEdit::GetInstance().showWindow)
						WindowTileMapEdit::GetInstance().DrawWindow();

					if (WindowTileMapBrush::GetInstance().showWindow)
						WindowTileMapBrush::GetInstance().DrawWindow();

					if (WindowTileMapViewer::GetInstance().showWindow)
						WindowTileMapViewer::GetInstance().DrawWindow();

					//FlipBook windows
					if (WindowFlipBookEdit::GetInstance().showWindow)
						WindowFlipBookEdit::GetInstance().DrawWindow();
				}
			}
			else if (openedTab->tabType == Utils::VisualScriptEditor) {
				//If openedTabType is script, draw script windows
				if (ServiceLocator::Get<IVisualScriptManager>()->GetCurrentScript()) {
					if (WindowVisualScript::GetInstance().showWindow) {
						WindowVisualScript::GetInstance().DrawWindow();
					}
				}
			}
		}

		if (WindowAllScenes::GetInstance().showWindow)
			WindowAllScenes::GetInstance().DrawWindow();

		if (WindowAddScene::GetInstance().showWindow)
			WindowAddScene::GetInstance().DrawWindow();

		if (WindowProjectSettings::GetInstance().showWindow)
			WindowProjectSettings::GetInstance().DrawWindow();
	}
	else {
		WindowProjectDialog::GetInstance().DrawWindow();
	}

	if (WindowEditorSettings::GetInstance().showWindow)
		WindowEditorSettings::GetInstance().DrawWindow(&darkTheme, [this]() { SetDarkTheme(); });
	//std::bind(&InterfaceManager::SetDarkTheme, &InterfaceManager::GetInstance()) //TODO: I MIGHT USE THIS


	/* Modal Window */

	WindowModalDialog::GetInstance().OpenModalIfNeeded();

	WindowModalDialog::GetInstance().DrawWindow();
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
