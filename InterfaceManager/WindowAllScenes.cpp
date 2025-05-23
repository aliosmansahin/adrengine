#include "pch.h"
#include "WindowAllScenes.h"

/*
PURPOSE: Draws the window
*/
void WindowAllScenes::DrawWindow(
	std::string& projectDir,
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
	bool& windowAddSceneShowWindow,
	Entity* windowEntityPropertiesCurrentEntity,
	std::string& windowSceneSelectedId,
	std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
	std::string& selectedTabId,
	Tab*& openedTab)
{
	//Begin the window
	ImGui::Begin("All Scenes", &showWindow);
	ImGui::SetWindowFontScale(1.5f);

	//Some text
	int windowWidth = (int)ImGui::GetWindowSize().x;
	int buttonWidth = 100;
	int padding = 10;
	ImGui::TextColored(ImVec4(0, 255, 0, 255), "Select and edit");
	ImGui::SameLine();
	ImGui::SetCursorPosX(float(windowWidth - buttonWidth - padding));

	//Add a new scene button
	if (ImGui::Button("Add", ImVec2((float)buttonWidth, 0.0f))) {
		windowAddSceneShowWindow = true;
	}
	ImGui::Separator();

	//For each scene, draw a selectable and handle when user selects one of them
	for (auto& sceneIter : SceneManager::GetInstance().scenes) {
		auto& scene = sceneIter.second;
		bool selected = scene == selectedSceneId;
		ImGui::Selectable(scene.c_str(), selected);

		//One click
		if(ImGui::IsItemClicked()) {
			if (ImGui::IsMouseDown(0)) {
				selectedSceneId = scene;
			}
		}

		//Check if the click is on one of the selectables
		if (ImGui::IsItemHovered()) {
			//Right click release
			if (ImGui::IsMouseReleased(1)) {
				ImGui::OpenPopup("ScenePopup");
				selectedSceneId = scene;
			}

			//Double click
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				selectedSceneId = scene;

				//Clear the previous scene editting stuff
				windowEntityPropertiesCurrentEntity = nullptr;
				windowSceneSelectedId = "";

				//Load the scene
				Scene* scenePtr = SceneManager::GetInstance().LoadScene(scene, projectDir, tabs, entityTypes);
				if (scenePtr) {
					//Change the selectedTab and currentScene
					auto iter = tabs.find(scene);
					if (iter != tabs.end()) {
						SceneManager::GetInstance().currentScene = scenePtr;
						openedTab = iter->second.get();
						selectedTabId = iter->second->id;
					}
				}
			}
		}
	}

	//Open a popup when the user right-clicks it
	if (ImGui::BeginPopup("ScenePopup")) {
		//When user deletes a scene, wait for the engine to delete it
		if (ImGui::MenuItem("Delete")) {
			pendingDelete = true;
		}
		ImGui::EndPopup();
	}

	//End the window
	ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAllScenes& WindowAllScenes::GetInstance()
{
	static WindowAllScenes window;
	return window;
}
