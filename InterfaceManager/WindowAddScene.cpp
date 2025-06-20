#include "pch.h"
#include "WindowAddScene.h"
#include "VisualScriptManager.h"

/*
PURPOSE: Draws the window
*/
void WindowAddScene::DrawWindow(
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs,
    Utils::Tab*& openedTab,
    std::string& selectedTabId)
{
    //Begin the window
    ImGui::Begin("Add Scene", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    ImGui::Text("Scene Type"); ImGui::SameLine();

    //Selection for the type of the scene
    static Utils::SceneType activeSceneType = Utils::SCENE_2D; //2D

    ImGui::RadioButton("2D", &(int&)activeSceneType, (int)Utils::SCENE_2D); ImGui::SameLine();
    ImGui::RadioButton("3D", &(int&)activeSceneType, (int)Utils::SCENE_3D);

    //TODO: Add Change Scene Name Here

    //When the user selects the add button
    if(ImGui::Button("Add")) {
        SceneManager::GetInstance().CreateScene(activeSceneType, projectDir);

        tabs.clear();

        //Create a new tab and insert it to all tabs
        Utils::Tab* tab = new Utils::Tab();
        tab->id = SceneManager::GetInstance().openedScene->sceneId;
        tab->tabType = Utils::SceneEditor;

        tabs.insert(std::pair<std::string, std::unique_ptr<Utils::Tab>>(tab->id, std::unique_ptr<Utils::Tab>(tab)));

        //Set the current scene and tab to the new scene and tab
        openedTab = tab;
        selectedTabId = tab->id;

        //Save the project
        std::string projectFile = projectDir + "project.adrengineproject";

        nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, SceneManager::GetInstance().openedScene->sceneId);

        AssetSaver::SaveProjectToFile(projectFile, projectJson);

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
