#include "pch.h"
#include "WindowProjectSettings.h"

/*
PURPOSE: Draws the window
TODO: This function will be rewrited to edit the settings of the project,
	but now shows only a demo window
*/
void WindowProjectSettings::DrawWindow()
{
	ImGui::ShowDemoWindow(&WindowProjectSettings::GetInstance().showWindow);
}

/*
PURPOSE: Gets the instance of the class
*/
WindowProjectSettings& WindowProjectSettings::GetInstance()
{
	static WindowProjectSettings window;
	return window;
}
