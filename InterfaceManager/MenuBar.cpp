#include "pch.h"
#include "MenuBar.h"

#include "ServiceLocator.h"
#include "interfaces/IProject/IProject.h"

#include "InterfaceManager.h"

/*
PURPOSE: Draws menu bar
*/
void MenuBar::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		auto projectInterface = ServiceLocator::Get<IProject>();

		if (projectInterface->GetProjectOpened()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save Project")) {
					projectInterface->SaveProject();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close Project")) {
					projectInterface->CloseProject();
					InterfaceManager::GetInstance().ResetInterface();
					WindowProjectDialog::GetInstance().ResetInputs();
				}
				ImGui::EndMenu();
			}
		}
		if (ImGui::BeginMenu("Layout")) {
			if (ImGui::MenuItem("Create a new default profile")) {
				WindowModalDialog::GetInstance().ShowModalQuestion("Change Layout Profile",
					"Are you sure you want to create a new profile?\nRestart required to apply profile changes",
					[]() {
						LayoutManager::GetInstance().CreateProfile();
						ServiceLocator::Get<IEngine>()->CloseEditor();
					}
				);
			}
			if (ImGui::MenuItem("Delete this profile")) {
				WindowModalDialog::GetInstance().ShowModalQuestion("Delete Profile", "Are you sure you want to delete this profile?\nRestart required to apply profile changes",
					[]() {
						LayoutManager::GetInstance().DeleteCurrentProfile();
						ServiceLocator::Get<IEngine>()->CloseEditor();
					}
				);
			}
			if(ImGui::MenuItem("Rename this profile")) {
				WindowModalDialog::GetInstance().ShowModalInput("Rename Profile", "Enter a new name for this profile:",
					[](std::string input) {
						LayoutManager::GetInstance().RenameCurrentProfile(input);
					}
				);
			}

			ImGui::SeparatorText("Profiles");
			for (auto& [id, name] : LayoutManager::GetInstance().GetProfilesList()) {
				if (ImGui::MenuItem(name.c_str(), NULL, LayoutManager::GetInstance().IsProfileSelected(id))) {
					WindowModalDialog::GetInstance().ShowModalQuestion("Change Layout Profile",
						"Are you sure you want to change profile to " + name + "?\nRestart required to apply profile changes",
						[&id]() {
							LayoutManager::GetInstance().ChangeProfile(id);
							ServiceLocator::Get<IEngine>()->CloseEditor();
						}
					);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(Localization::GetString("interface_main_menu_window"))) {
			if (projectInterface->GetProjectOpened()) {
				ImGui::MenuItem(Localization::GetString("interface_main_menu_item_scene"), NULL, &WindowScene::GetInstance().showWindow);
				ImGui::MenuItem("All Scenes", NULL, &WindowAllScenes::GetInstance().showWindow);
				ImGui::MenuItem(Localization::GetString("interface_main_menu_item_game_viewport"), NULL, &WindowGameViewport::GetInstance().showWindow);
				ImGui::MenuItem(Localization::GetString("interface_main_menu_item_entity_properties"), NULL, &WindowEntityProperties::GetInstance().showWindow);
				ImGui::Separator();
				ImGui::MenuItem(Localization::GetString("interface_main_menu_item_project_settings"), NULL, &WindowProjectSettings::GetInstance().showWindow);
			}
			ImGui::MenuItem(Localization::GetString("interface_main_menu_item_editor_settings"), NULL, &WindowEditorSettings::GetInstance().showWindow);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

/*
PURPOSE: Gets the instance of the class
*/
MenuBar& MenuBar::GetInstance()
{
	static MenuBar menubar;
	return menubar;
}
