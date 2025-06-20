#include "pch.h"
#include "WindowGameViewport.h"
#include "SceneManager.h"
#include "VisualScriptManager.h"
#include "WindowTileMapBrush.h"

/*
PURPOSE: Draws the window
*/
void WindowGameViewport::DrawWindow(
    float engineFPS,
    float engineMS)
{
    //We will use tileMapBrush when "start drawing" button clicked
    TileMap* edittingTileMap = WindowTileMapBrush::GetInstance().editing ? WindowTileMapBrush::GetInstance().editingTileMap : nullptr;

    //Begin the window
    ImGui::Begin("Game Viewport", &showWindow);

    //Store if the window is focused
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // The window will be focused on right click
    }
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
            SceneManager::GetInstance().openedScene->GetEntityManager()->RunEntitiesScriptBegin();
            isPlaying = true;
        }
    }
    ImGui::SameLine();
    
    ImGui::SetWindowFontScale(1.2f);

    //Draw the stats
    std::string fpsMsInfo = "FPS: " + std::to_string(engineFPS) + " / " + "Frame Rate: " + std::to_string(engineMS) + "ms / ";
    ImGui::Text(fpsMsInfo.c_str());

    ImGui::SameLine();

    std::string cameraInfo = "CamX: " + std::to_string(SceneManager::GetInstance().openedScene->currentCamera->GetEntityParams()->x) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    cameraInfo = "CamY: " + std::to_string(SceneManager::GetInstance().openedScene->currentCamera->GetEntityParams()->y) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    if (SceneManager::GetInstance().openedScene->sceneType == Utils::SCENE_3D) {
        cameraInfo = "CamZ: " + std::to_string(SceneManager::GetInstance().openedScene->currentCamera->GetEntityParams()->z) + " / ";
        ImGui::Text(cameraInfo.c_str());
        ImGui::SameLine();
    }

    ImGui::SetWindowFontScale(1.5f);
    
    ImGui::EndChild();

    //Some sets
    window_width = ImGui::GetContentRegionAvail().x;
    window_height = ImGui::GetContentRegionAvail().y;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    //Check the mouse is in the texture
    ImVec2 mousePos = ImGui::GetMousePos();
    if (mousePos.x > pos.x && mousePos.x < pos.x + window_width &&
        mousePos.y > pos.y && mousePos.y < pos.y + window_height) {
        isHovered = true;
    }
    else
        isHovered = false;

    //Handle for resizing
    Graphics::GetInstance().GetMainFramebuffer()->RescaleFramebuffer((int)window_width, (int)window_height);
    adr_glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);

    /*
        Draw the scene, this function stores the frame into the frameBufferTex texture,
        after that, draw the texture as a texture via ImGui
    */
    SceneManager::GetInstance().openedScene->DrawScene(
        (int)window_width,
        (int)window_height);

    //Draw the main frameBuffer texture as an image
    ImGui::Image((ImTextureID)(intptr_t)Graphics::GetInstance().GetMainFramebuffer()->GetFrameBufferTex(),
        ImVec2(window_width, window_height), ImVec2(0, 1), ImVec2(1, 0));

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
