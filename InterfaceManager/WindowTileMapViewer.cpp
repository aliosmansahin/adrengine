#include "pch.h"
#include "WindowTileMapViewer.h"

INTERFACEMANAGER_API void WindowTileMapViewer::DrawWindow(int screenWidth, int screenHeight)
{
	//Check for a tile is editting
	if (!edittingTileMap)
		return;
	TileMapParams* params = dynamic_cast<TileMapParams*>(edittingTileMap->GetEntityParams());
	if (!params)
		return;

	ImGui::Begin("TileMap Viewer", &showWindow);


    //Draw the toolbar to draw some stats
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

    ImGui::SetWindowFontScale(1.2f);

    //Draw the stats
    std::string cameraInfo = "CamX: " + std::to_string(cameraX) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    cameraInfo = "CamY: " + std::to_string(cameraY) + " / ";
    ImGui::Text(cameraInfo.c_str());
    ImGui::SameLine();

    ImGui::SetWindowFontScale(1.5f);

    ImGui::EndChild();

    //Some sets
    window_width = ImGui::GetContentRegionAvail().x;
    window_height = ImGui::GetContentRegionAvail().y;

    //Store current camera position in a vector
    glm::vec3 currentSceneCameraPos = glm::vec3(cameraX, cameraY, 0.0f);

    //Handle for resizing
    Graphics::GetInstance().RescaleFramebuffer((int)window_width, (int)window_height);

    //Save the previous shader type
    Utils::ShaderType oldShaderType = ShaderManager::GetInstance().GetCurrentType();

    //Bind the framebuffer
    Graphics::GetInstance().BindFramebuffer();
    Graphics::GetInstance().Clear();
    adr_glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
    ShaderManager::GetInstance().UseShaders(Utils::SHADER_2D);
    ShaderManager::GetInstance().UpdateTransformMatrix2D((int)window_width, (int)window_height, (int)cameraX, (int)cameraY);

    edittingTileMap->Draw(currentSceneCameraPos);

    //Disable the frame buffer to draw ImGui image
    Graphics::GetInstance().UnbindFramebuffer();
    ShaderManager::GetInstance().UseShaders(oldShaderType);

    //Draw the frameBufferTex texture as an image
    ImVec2 pos = ImGui::GetCursorScreenPos();

    bool isFocused = ImGui::IsWindowFocused();

    ImGui::GetWindowDrawList()->AddImage(
        (ImTextureID)(intptr_t)Graphics::GetInstance().GetFrameBufferTex(),
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImVec2 mousePos = ImGui::GetMousePos();

    //Check the mouse is in the texture
    if (mousePos.x > pos.x && mousePos.x < pos.x + window_width &&
        mousePos.y > pos.y && mousePos.y < pos.y + window_height) {
        isHovered = true;
    }
    else
        isHovered = false;

    //Calculate mouse position related to window
    ImVec2 localMousePos = ImVec2(
        mousePos.x - pos.x,
        mousePos.y - pos.y
    );

    //Translate mousePos to Normalized Device Coordinates(NDC)
    float ndcX = (localMousePos.x / window_width) * 2.0f - 1.0f;
    float ndcY = 1.0f - (localMousePos.y / window_height) * 2.0f;

    //Clip scene vector from NDC
    glm::vec4 mouseClip = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    //Get projection matrix and invert it in order to get world coordinates
    glm::mat4 ortho = ShaderManager::GetInstance().GetProjectionMatrix2D((int)window_width, (int)window_height);
    glm::mat4 invOrtho = glm::inverse(ortho);

    //Calculate world mouse coordinates
    glm::vec4 worldMouse = invOrtho * mouseClip;

    //Set the mouse coordinates
    mouseX = (int)worldMouse.x;
    mouseY = (int)worldMouse.y;

    cameraX -= SceneManager::GetInstance().currentScene->deltaX;
    cameraY -= SceneManager::GetInstance().currentScene->deltaY;

    if (isHovered) {
        //Update tile indicator
        edittingTileMap->UpdateMouseTileIndicator(mouseX, mouseY, cameraX, cameraY);
    }

    if (isFocused) {
        if (SceneManager::GetInstance().currentScene->leftPressed) {
            //TileMap will add a tile to its own tiles
            edittingTileMap->AddTileToMap(mouseX, mouseY, cameraX, cameraY, WindowTileMapBrush::GetInstance().selectedTile);
        }
        if (SceneManager::GetInstance().currentScene->deletePressed) {
            //TileMap will remove the tile from its own tiles
            edittingTileMap->RemoveTileFromMap(mouseX, mouseY, cameraX, cameraY);
        }
    }

	ImGui::End();
}

INTERFACEMANAGER_API WindowTileMapViewer& WindowTileMapViewer::GetInstance()
{
	static WindowTileMapViewer window;
	return window;
}
