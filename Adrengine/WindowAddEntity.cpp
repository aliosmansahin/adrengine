#include "WindowAddEntity.h"

void WindowAddEntity::DrawWindow()
{
	ImGui::Begin("Add Entity", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowFontScale(1.5f);
	//ImGui::TextColored(ImVec4(0, 255, 0, 255), "Search for type");

	if (shouldFocus) {
		ImGui::SetKeyboardFocusHere();
		shouldFocus = false;
	}
	ImGui::InputTextWithHint("Entity Type", "Search for type", typeBuf, sizeof(typeBuf));

	if (ImGui::IsItemEdited()) {
		types.clear();
		typeBuf[sizeof(typeBuf) - 1] = '\0';
		std::string search = typeBuf;
		if (!search.empty()) {
			std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) {
				return std::toupper(c);
				});

			for (auto& type : Engine::GetInstance().entityTypes) {
				std::string typeToUp = type.first;
				std::transform(typeToUp.begin(), typeToUp.end(), typeToUp.begin(), [](unsigned char c) {
					return std::toupper(c);
					});
				if (typeToUp.compare(0, search.length(), search) == 0) {
					types.push_back(type.first);
				}
			}
		}
	}

	ImGui::Separator();
	ImGui::BeginChild("Results");
	int padding = 10;
	for (auto& type : types) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
		if (ImGui::Selectable(type.c_str())) {
			std::string id = SceneManager::GetInstance().currentScene->GetEntityManager()->CreateEntity(type, WindowScene::GetInstance().addParent);
			std::string findId = SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities().find(id)->second->GetEntityParams()->id;
			showWindow = false;
			memset(typeBuf, 0, sizeof(typeBuf));
			types.clear();
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

WindowAddEntity& WindowAddEntity::GetInstance()
{
	static WindowAddEntity window;
	return window;
}
