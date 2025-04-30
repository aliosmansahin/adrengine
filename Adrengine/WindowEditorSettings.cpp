#include "WindowEditorSettings.h"

void WindowEditorSettings::DrawWindow()
{
    ImGui::Begin(Localization::Get().GetString("interface_main_menu_item_editor_settings"), &showWindow, ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFontScale(1.5f);

    if (ImGui::Checkbox("Dark Theme", &InterfaceManager::GetInstance().darkTheme)) {
        InterfaceManager::GetInstance().SetDarkTheme();
    }

    int contentX = (int)ImGui::GetContentRegionAvail().x;
    int contentY = (int)ImGui::GetContentRegionAvail().y;
    int buttonWidth = 100;
    int buttonHeight = 50;
    int padding = 10;

    int startY = contentY - buttonHeight + padding * 4;
    ImGui::SetCursorPosY((float)startY);
    ImGui::Separator();
    int startX = contentX - buttonWidth;
    startY += padding;
    ImGui::SetCursorPosX((float)startX);
    ImGui::SetCursorPosY((float)startY);
    ImGui::Button("Done", ImVec2((float)buttonWidth, (float)buttonHeight));

    ImGui::End();
}

WindowEditorSettings& WindowEditorSettings::GetInstance()
{
    static WindowEditorSettings window;
    return window;
}
