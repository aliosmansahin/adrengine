#include "MenuBar.h"

void MenuBar::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save Project")) {
				Engine::GetInstance().SaveProject();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(Localization::Get().GetString("interface_main_menu_window"))) {
			ImGui::MenuItem(Localization::Get().GetString("interface_main_menu_item_scene"), NULL, &WindowScene::GetInstance().showWindow);
			ImGui::MenuItem("All Scenes", NULL, &WindowAllScenes::GetInstance().showWindow);
			ImGui::MenuItem(Localization::Get().GetString("interface_main_menu_item_game_viewport"), NULL, &WindowGameViewport::GetInstance().showWindow);
			ImGui::MenuItem(Localization::Get().GetString("interface_main_menu_item_entity_properties"), NULL, &WindowEntityProperties::GetInstance().showWindow);
			ImGui::Separator();
			ImGui::MenuItem(Localization::Get().GetString("interface_main_menu_item_project_settings"), NULL, &WindowProjectSettings::GetInstance().showWindow);
			ImGui::MenuItem(Localization::Get().GetString("interface_main_menu_item_editor_settings"), NULL, &WindowEditorSettings::GetInstance().showWindow);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

MenuBar& MenuBar::GetInstance()
{
	static MenuBar menubar;
	return menubar;
}
