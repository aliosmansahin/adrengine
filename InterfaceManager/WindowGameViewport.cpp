#include "pch.h"
#include "WindowGameViewport.h"
#include "SceneManager.h"
#include "VisualScriptManager.h"

/*
PURPOSE: Draws the window
*/
void WindowGameViewport::DrawWindow(
    float engineFPS,
    float engineMS)
{
    //Store current camera position in a vector
    glm::vec3 currentSceneCameraPos = glm::vec3(SceneManager::GetInstance().currentScene->cameraX, SceneManager::GetInstance().currentScene->cameraY, SceneManager::GetInstance().currentScene->cameraZ);

    /*
        Draw the scene, this function stores the frame into the frameBufferTex texture,
        after that, draw the texture as a texture via ImGui
    */
    SceneManager::GetInstance().currentScene->DrawScene((int)window_width, (int)window_height, currentSceneCameraPos);

    //Begin the window
    ImGui::Begin("Game Viewport", &showWindow);

    //Store if the window is focused
    isFocused = ImGui::IsWindowFocused();

    //Draw the toolbar to draw some buttons like play/stop and some stats
    int toolbarHeight = 30;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    //ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + InterfaceManager::GetInstance().tabHeight));
    //ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, toolbarHeight));
    ////ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::BeginChild("Toolbar", ImVec2(viewport->WorkSize.x, (float)toolbarHeight), 0, window_flags);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::PopStyleVar(3);

    //Every selections, play/stop will be changed
    if (isPlaying) {
        if (ImGui::Selectable("Stop", false, ImGuiSelectableFlags_None, ImVec2(100, (float)toolbarHeight))) {
            isPlaying = false;
        }
    }
    else {
        if (ImGui::Selectable("Play", false, ImGuiSelectableFlags_None, ImVec2(100, (float)toolbarHeight))) {
            //When user plays the scene, run the scripts
            VisualScriptManager::GetInstance().RunScriptsBegin();
            isPlaying = true;
        }
    }
    ImGui::SameLine();
    
    ImGui::SetWindowFontScale(1.2f);

    //Draw the stats
    std::string fpsMsInfo = "FPS: " + std::to_string(engineFPS) + " / " + "Frame Rate: " + std::to_string(engineMS) + "ms / ";
    ImGui::Text(fpsMsInfo.c_str());

    ImGui::SameLine();

    std::string cameraInfo = "CamX: " + std::to_string(SceneManager::GetInstance().currentScene->cameraX) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    cameraInfo = "CamY: " + std::to_string(SceneManager::GetInstance().currentScene->cameraY) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    if (SceneManager::GetInstance().currentScene->sceneType == SCENE_3D) {
        cameraInfo = "CamZ: " + std::to_string(SceneManager::GetInstance().currentScene->cameraZ) + " / ";
        ImGui::Text(cameraInfo.c_str());
        ImGui::SameLine();
    }

    ImGui::SetWindowFontScale(1.5f);
    
    ImGui::EndChild();

    //Some sets
    window_width = ImGui::GetContentRegionAvail().x;
    window_height = ImGui::GetContentRegionAvail().y;

    //Handle for resizing
    Graphics::GetInstance().RescaleFramebuffer((int)window_width, (int)window_height);
    glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);

    //Update the transform matrix depends on the scene type
    if (SceneManager::GetInstance().currentScene->sceneType == SCENE_2D)
        ShaderManager::GetInstance().UpdateTransformMatrix2D((int)window_width, (int)window_height, (int)SceneManager::GetInstance().currentScene->cameraX, (int)SceneManager::GetInstance().currentScene->cameraY);
    else if (SceneManager::GetInstance().currentScene->sceneType == SCENE_3D)
        ShaderManager::GetInstance().UpdateTransformMatrix3D(SceneManager::GetInstance().currentScene->eye, (int)window_width, (int)window_height, SceneManager::GetInstance().currentScene->cameraX, SceneManager::GetInstance().currentScene->cameraY, SceneManager::GetInstance().currentScene->cameraZ, SceneManager::GetInstance().currentScene->yaw, SceneManager::GetInstance().currentScene->pitch);

    //Draw the frameBufferTex texture as an image
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (ImTextureID)(intptr_t)Graphics::GetInstance().GetFrameBufferTex(),
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    //Check the mouse is in the texture
    ImVec2 mousePos = ImGui::GetMousePos();
    if (mousePos.x > pos.x && mousePos.x < pos.x + window_width &&
        mousePos.y > pos.y && mousePos.y < pos.y + window_height) {
        isHovered = true;
    }
    else
        isHovered = false;

    //End the window
    ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowGameViewport& WindowGameViewport::GetInstance()
{
	static WindowGameViewport window;
	return window;
}
