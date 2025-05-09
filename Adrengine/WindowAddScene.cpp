#include "WindowAddScene.h"

void WindowAddScene::DrawWindow()
{
    ImGui::Begin("Add Scene", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    ImGui::Text("Scene Type"); ImGui::SameLine();

    static SceneType activeSceneType = SCENE_2D; //2D

    ImGui::RadioButton("2D", &(int&)activeSceneType, (int)SCENE_2D); ImGui::SameLine();
    ImGui::RadioButton("3D", &(int&)activeSceneType, (int)SCENE_3D);

    //Add Change Scene Name


    if(ImGui::Button("Add")) {
        SceneManager::GetInstance().CreateScene(activeSceneType); // pass activeSceneType as parameter
        WindowEntityProperties::GetInstance().currentEntity = nullptr;
        WindowScene::GetInstance().selectedId = "";
        showWindow = false;
    }

    ImGui::End();
}

WindowAddScene& WindowAddScene::GetInstance()
{
    static WindowAddScene window;
    return window;
}
