#include "pch.h"
#include "WindowEditorSettings.h"

/*
PURPOSE: Draws the window
*/
void WindowEditorSettings::DrawWindow(bool* darkTheme, std::function<void()> setDarkThemeFunc)
{
    //Begin the window
    ImGui::Begin(Localization::GetString("interface_main_menu_item_editor_settings"), &showWindow, ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFontScale(1.5f);

    //Checkbox for the theme, if user selects it, the theme will be dark
    if (ImGui::Checkbox("Dark Theme", darkTheme)) {
        setDarkThemeFunc();
    }

    //Variables for the sub-buttons like "Done" button
    int contentX = (int)ImGui::GetContentRegionAvail().x;
    int contentY = (int)ImGui::GetContentRegionAvail().y;
    int buttonWidth = 100;
    int buttonHeight = 50;
    int padding = 10;

    //Draw the "Done" button
    int startY = contentY - buttonHeight + padding * 4;
    ImGui::SetCursorPosY((float)startY);
    ImGui::Separator();
    int startX = contentX - buttonWidth;
    startY += padding;
    ImGui::SetCursorPosX((float)startX);
    ImGui::SetCursorPosY((float)startY);
    ImGui::Button("Done", ImVec2((float)buttonWidth, (float)buttonHeight));
    
    //End the window
    ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowEditorSettings& WindowEditorSettings::GetInstance()
{
    static WindowEditorSettings window;
    return window;
}
