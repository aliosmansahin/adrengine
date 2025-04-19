#include "MenuBar.h"

void MenuBar::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("DemoWindow", NULL, &WindowProjectSettings::GetInstance().showWindow);
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
