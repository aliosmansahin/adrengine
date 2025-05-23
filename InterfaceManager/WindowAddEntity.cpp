#include "pch.h"
#include "WindowAddEntity.h"

/*
PURPOSE: Draws window
*/
void WindowAddEntity::DrawWindow(
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
	std::string& projectDir,
	std::shared_ptr<Entity>& windowSceneAddParent)
{
	//Begin window
	ImGui::Begin("Add Entity", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowFontScale(1.5f);
	//ImGui::TextColored(ImVec4(0, 255, 0, 255), "Search for type");

	/*
		In first frame,
		focus on the search input
	*/
	if (shouldFocus) {
		ImGui::SetKeyboardFocusHere();
		shouldFocus = false;
	}

	//Inputbox
	ImGui::InputTextWithHint("Entity Type", "Search for type", typeBuf, sizeof(typeBuf));

	/*
		After every edit, search for the entity:
			If the name of the entity starts with the input,
			add it to the search results
	*/
	if (ImGui::IsItemEdited()) {
		//Clear previous search results
		types.clear();
		
		//Zero termination
		typeBuf[sizeof(typeBuf) - 1] = '\0';

		//Pass the input buffer into a string
		std::string search = typeBuf;

		//Check for if search input isn't empty
		if (!search.empty()) {
			std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) {
				return std::toupper(c);
				});

			//Compare each entity type with the input
			for (auto& type : entityTypes) {
				std::string typeToUp = type.first;
				std::transform(typeToUp.begin(), typeToUp.end(), typeToUp.begin(), [](unsigned char c) {
					return std::toupper(c);
					});

				//If they match, add type into results
				if (typeToUp.compare(0, search.length(), search) == 0) {
					types.push_back(type.first);
				}
			}
		}
	}

	ImGui::Separator();

	//Child window for the results
	ImGui::BeginChild("Results");
	int padding = 10;

	//Draw each result type
	for (auto& type : types) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
		if (!type.empty()) {

			//If the user selects a type
			if (ImGui::Selectable(type.c_str())) {
				nlohmann::json sceneJson = SceneManager::GetInstance().currentScene->ToJson();
				std::string id = SceneManager::GetInstance().currentScene->GetEntityManager()->CreateEntity(type, projectDir, entityTypes, SceneManager::GetInstance().currentScene->sceneId, sceneJson, windowSceneAddParent);
				auto iter = SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities().find(id);
				if (id != "" && iter != SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities().end()) {
					std::string findId = iter->second->GetEntityParams()->id;
					showWindow = false;
					memset(typeBuf, 0, sizeof(typeBuf));
					types.clear();
				}
			}
		}
	}

	//End window
	ImGui::EndChild();
	ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAddEntity& WindowAddEntity::GetInstance()
{
	static WindowAddEntity window;
	return window;
}
