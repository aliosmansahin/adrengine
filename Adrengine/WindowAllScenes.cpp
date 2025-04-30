#include "WindowAllScenes.h"

void WindowAllScenes::DrawWindow()
{
	ImGui::Begin("All Scenes", &showWindow);
	ImGui::SetWindowFontScale(1.5f);
	int windowWidth = (int)ImGui::GetWindowSize().x;
	int buttonWidth = 100;
	int padding = 10;
	ImGui::TextColored(ImVec4(0, 255, 0, 255), "Select and edit");
	ImGui::SameLine();
	ImGui::SetCursorPosX(float(windowWidth - buttonWidth - padding));
	if (ImGui::Button("Add", ImVec2((float)buttonWidth, 0.0f))) {
	 	SceneManager::GetInstance().CreateScene();
		WindowEntityProperties::GetInstance().currentEntity = nullptr;
		WindowScene::GetInstance().selectedId = "";
	}
	ImGui::Separator();

	for (auto& sceneIter : SceneManager::GetInstance().scenes) {
		auto& scene = sceneIter.second;
		bool selected = scene == selectedSceneId;
		ImGui::Selectable(scene.c_str(), selected);
		if(ImGui::IsItemClicked()) {
			if (ImGui::IsMouseDown(0)) {
				selectedSceneId = scene;
			}
		}
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseReleased(1)) {
				ImGui::OpenPopup("ScenePopup");
				selectedSceneId = scene;
			}
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				selectedSceneId = scene;
				WindowEntityProperties::GetInstance().currentEntity = nullptr;
				WindowScene::GetInstance().selectedId = "";
				Scene* scenePtr = SceneManager::GetInstance().LoadScene(scene);
				if (scenePtr) {
					auto iter = InterfaceManager::GetInstance().tabs.find(scene);
					if (iter != InterfaceManager::GetInstance().tabs.end()) {
						InterfaceManager::GetInstance().selectedTabId = selectedSceneId;
						SceneManager::GetInstance().currentScene = scenePtr;
						InterfaceManager::GetInstance().openedTab = iter->second.get();
						InterfaceManager::GetInstance().selectedTabId = iter->second->id;
					}
				}
			}
		}
	}

	if (ImGui::BeginPopup("ScenePopup")) {
		if (ImGui::MenuItem("Delete")) {
			pendingDelete = true;
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

WindowAllScenes& WindowAllScenes::GetInstance()
{
	static WindowAllScenes window;
	return window;
}
