#include "pch.h"
#include "WindowAddEntity.h"

#include "WindowScene.h"

/*
PURPOSE: Draws window
*/
void WindowAddEntity::DrawWindow()
{
	float posX = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().first - defSize.x) / 2.0f;
	float posY = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().second - defSize.y) / 2.0f;

	ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(defSize, ImGuiCond_FirstUseEver);

	//Begin window
	ImGui::Begin(GetWindowTitleWithID().c_str(), &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
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
			for (auto& type : ServiceLocator::Get<IEngine>()->GetEntityTypes()) {
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
				nlohmann::json sceneJson = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->ToJson();
				std::string id = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->CreateEntity(type, ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetSceneId(), sceneJson, WindowScene::GetInstance().addParent);
				auto iter = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->GetEntities().find(id);
				if (id != "" && iter != ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->GetEntities().end()) {
					std::string findId = iter->second->GetEntityParams()->GetId();
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
