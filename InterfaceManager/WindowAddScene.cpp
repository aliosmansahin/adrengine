#include "pch.h"
#include "WindowAddScene.h"
#include "VisualScriptManager.h"

/*
PURPOSE: Draws the window
*/
void WindowAddScene::DrawWindow(
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::string& selectedTabId)
{
    //Begin the window
    ImGui::Begin("Add Scene", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    ImGui::Text("Scene Type"); ImGui::SameLine();

    //Selection for the type of the scene
    static SceneType activeSceneType = SCENE_2D; //2D

    ImGui::RadioButton("2D", &(int&)activeSceneType, (int)SCENE_2D); ImGui::SameLine();
    ImGui::RadioButton("3D", &(int&)activeSceneType, (int)SCENE_3D);

    //TODO: Add Change Scene Name Here

    //When the user selects the add button
    if(ImGui::Button("Add")) {
        SceneManager::GetInstance().CreateScene(activeSceneType, projectDir, tabs, openedTab, selectedTabId, VisualScriptManager::GetInstance().scripts); // pass activeSceneType as parameter
        WindowEntityProperties::GetInstance().currentEntity = nullptr;
        WindowScene::GetInstance().selectedId = "";
        showWindow = false;
    }

    //End the window
    ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAddScene& WindowAddScene::GetInstance()
{
    static WindowAddScene window;
    return window;
}
