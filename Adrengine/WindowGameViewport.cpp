#include "WindowGameViewport.h"

void WindowGameViewport::DrawWindow()
{
    Graphics::GetInstance().BindFramebuffer();
    //Draw the game here
    Graphics::GetInstance().Clear();
    SceneManager::GetInstance().currentScene->DrawScene();

    Graphics::GetInstance().UnbindFramebuffer();

    ImGui::Begin("Game Viewport", &showWindow);


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
    ImGui::BeginChild("Toolbar", ImVec2(viewport->WorkSize.x, toolbarHeight), 0, window_flags);
    ImGui::SetWindowFontScale(1.5f);

    ImGui::PopStyleVar(3);

    if (isPlaying) {
        if (ImGui::Selectable("Stop", false, ImGuiSelectableFlags_None, ImVec2(100, toolbarHeight))) {
            isPlaying = false;
        }
    }
    else {
        if (ImGui::Selectable("Play", false, ImGuiSelectableFlags_None, ImVec2(100, toolbarHeight))) {
            VisualScriptManager::GetInstance().RunScriptsBegin();
            isPlaying = true;
        }
    }

    ImGui::EndChild();

    window_width = ImGui::GetContentRegionAvail().x;
    window_height = ImGui::GetContentRegionAvail().y;
    Graphics::GetInstance().RescaleFramebuffer(window_width, window_height);
    glViewport(0, 0, window_width, window_height);
    ShaderManager::GetInstance().UpdateProjectionMatrix(window_width, window_height, SceneManager::GetInstance().currentScene->cameraX, SceneManager::GetInstance().currentScene->cameraY);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (ImTextureID)(intptr_t)Graphics::GetInstance().GetFrameBufferTex(),
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImVec2 mousePos = ImGui::GetMousePos();
    if (mousePos.x > pos.x && mousePos.x < pos.x + window_width &&
        mousePos.y > pos.y && mousePos.y < pos.y + window_height) {
        isHovered = true;
    }
    else
        isHovered = false;

    ImGui::End();
}

WindowGameViewport& WindowGameViewport::GetInstance()
{
	static WindowGameViewport window;
	return window;
}
