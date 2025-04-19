#include "WindowProjectSettings.h"

void WindowProjectSettings::DrawWindow()
{
	ImGui::ShowDemoWindow(&WindowProjectSettings::GetInstance().showWindow);
}

WindowProjectSettings& WindowProjectSettings::GetInstance()
{
	static WindowProjectSettings window;
	return window;
}
