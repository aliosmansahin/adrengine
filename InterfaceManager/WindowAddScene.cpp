#include "pch.h"
#include "WindowAddScene.h"

#include "WindowEntityProperties.h"
#include "WindowScene.h"

#include "AssetSaver.h"

#include "interfaces/IProject/IProject.h"
#include "interfaces/IEngine/IEngine.h"

#include "InterfaceManager.h"

/*
PURPOSE: Draws the window
*/
void WindowAddScene::DrawWindow()
{
    float posX = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().first - defSize.x) / 2.0f;
    float posY = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().second - defSize.y) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(defSize, ImGuiCond_FirstUseEver);

    //Begin the window
    ImGui::Begin(GetWindowTitleWithID().c_str(), &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    ImGui::Text("Scene Type"); ImGui::SameLine();

    //Selection for the type of the scene
    static Utils::SceneType activeSceneType = Utils::SCENE_2D; //2D

    ImGui::RadioButton("2D", &(int&)activeSceneType, (int)Utils::SCENE_2D); ImGui::SameLine();
    ImGui::RadioButton("3D", &(int&)activeSceneType, (int)Utils::SCENE_3D);

    //TODO: Add Change Scene Name Here

    //When the user selects the add button
    if(ImGui::Button("Add")) {
        auto scene = ServiceLocator::Get<ISceneManager>()->CreateScene(activeSceneType);

		//Remove all previous tabs
        InterfaceManager::GetInstance().RemoveAllTabs();

        //Create a new tab and insert it to all tabs
		InterfaceManager::GetInstance().AddTab(scene->GetSceneId(), Utils::SceneEditor);

        //Set the current scene and tab to the new scene and tab
		InterfaceManager::GetInstance().ActivateTab(scene->GetSceneId());

        //Save the project
        nlohmann::json projectJson = Utils::CreateProjectJson(ServiceLocator::Get<ISceneManager>()->GetScenes(), scene->GetSceneId());

        AssetSaver::SaveProjectToFile(ServiceLocator::Get<IProject>()->GetProjectFileLocation(), projectJson);

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
