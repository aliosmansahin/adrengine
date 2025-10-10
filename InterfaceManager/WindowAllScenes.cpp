#include "pch.h"
#include "WindowAllScenes.h"

#include "WindowAddScene.h"
#include "WindowEntityProperties.h"
#include "WindowScene.h"

#include "InterfaceManager.h"

/*
PURPOSE: Draws the window
*/
void WindowAllScenes::DrawWindow()
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
		WindowAddScene::GetInstance().showWindow = true;
	}
	ImGui::Separator();

	//For each scene, draw a selectable and handle when user selects one of them
	for (auto& sceneIter : ServiceLocator::Get<ISceneManager>()->GetScenes()) {
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
				WindowEntityProperties::GetInstance().currentEntity = nullptr;
				WindowScene::GetInstance().selectedId = "";

				//Load the scene
				std::shared_ptr<IScene> scenePtr = ServiceLocator::Get<ISceneManager>()->LoadScene(scene);
				if (scenePtr) {
					//Remove all previous tabs
					InterfaceManager::GetInstance().RemoveAllTabs();

					//Create a new tab
					InterfaceManager::GetInstance().AddTab(scene, Utils::SceneEditor);

					//Activate the tab
					InterfaceManager::GetInstance().ActivateTab(scene);
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
