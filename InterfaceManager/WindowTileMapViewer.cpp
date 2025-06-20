#include "pch.h"
#include "WindowTileMapViewer.h"

/*
PURPOSE: Draws the window
*/
INTERFACEMANAGER_API void WindowTileMapViewer::DrawWindow(int screenWidth, int screenHeight)
{
	//Check for a tile is editting
    if (!edittingTileMap) {
        showWindow = false;
        return;
    }
	TileMapParams* params = dynamic_cast<TileMapParams*>(edittingTileMap->GetEntityParams());
    if (!params) {
        showWindow = false;
        return;
    }

    static bool first = true;
    if (first) {
        //create a frame buffer to write it to imgui as a texture
        framebuffer = std::make_unique<FramebufferProvider>();
        framebuffer->CreateFramebuffer(800, 600);
        first = false;
    }

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
    window_width = (int)ImGui::GetContentRegionAvail().x;
    window_height = (int)ImGui::GetContentRegionAvail().y;

    //Store if the window is focused
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::SetWindowFocus(); // The window will be focused on right click
    }
    bool isFocused = ImGui::IsWindowFocused();

    //Store current camera position in a vector
    glm::vec3 currentSceneCameraPos = glm::vec3(cameraX, cameraY, 0.0f);

    //Some positions
    ImVec2 pos = ImGui::GetCursorScreenPos();
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

    //Inverting y-axis of viewer. See calling UpdateTransformMatrix2D below!
    if (SceneManager::GetInstance().openedScene->sceneType == Utils::SCENE_3D)
        ndcY = -ndcY;

    //Clip scene vector from NDC
    glm::vec4 mouseClip = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    //Get projection matrix and invert it in order to get world coordinates
    glm::mat4 ortho = ShaderManager::GetInstance().GetProjectionMatrix2D((int)window_width, (int)window_height);
    ortho = glm::inverse(ortho);

    //Calculate world mouse coordinates
    glm::vec4 worldMouse = ortho * mouseClip;

    //Set the mouse coordinates
    mouseX = (int)worldMouse.x;
    mouseY = (int)worldMouse.y;

    if (isFocused) {
        //Update the camera for tilemapviewer
        cameraX -= SceneManager::GetInstance().openedScene->deltaX;

        //Inverting y-axis of the camera. See calling UpdateTransformMatrix2D below!
        if (SceneManager::GetInstance().openedScene->sceneType == Utils::SCENE_3D)
            cameraY += SceneManager::GetInstance().openedScene->deltaY;
        else
            cameraY -= SceneManager::GetInstance().openedScene->deltaY;
    }

    if (isHovered) {
        //Update tile indicator
        edittingTileMap->UpdateMouseTileIndicator(mouseX, mouseY, cameraX, cameraY);
    }

    //Handle for resizing
    framebuffer->RescaleFramebuffer((int)window_width, (int)window_height);

    //Save the previous shader type
    Utils::ShaderType oldShaderType = ShaderManager::GetInstance().GetCurrentType();

    //Bind the framebuffer
    framebuffer->BindFramebuffer();
    Graphics::GetInstance().Clear();
    adr_glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
    ShaderManager::GetInstance().UseShaders(Utils::SHADER_2D);
    /*
        The y-axis of 3d scene and viewer are inverted to each other
        If the tilemap belongs to a 3d scene,
        we are inverting y-axis of the projection matrix.
    */
    ShaderManager::GetInstance().UpdateTransformMatrix2D(
        (int)window_width, (int)window_height,
        (int)cameraX, (int)cameraY,
        (SceneManager::GetInstance().openedScene->sceneType == Utils::SCENE_3D));

    //Set the drawing mode of the tilemap
    edittingTileMap->drawingViewer = true;

    //Render the tilemap to edit
    edittingTileMap->Draw(currentSceneCameraPos);

    //Disable the drawing mode of the tilemap
    edittingTileMap->drawingViewer = false;

    //Disable the frame buffer to draw ImGui image
    framebuffer->UnbindFramebuffer();
    ShaderManager::GetInstance().UseShaders(oldShaderType);

    //Draw the frameBuffer texture as an image
    ImGui::Image((ImTextureID)(intptr_t)framebuffer->GetFrameBufferTex(),
        ImVec2((float)window_width, (float)window_height), ImVec2(0, 1), ImVec2(1, 0));

    if (isFocused && isHovered) {
        if (SceneManager::GetInstance().openedScene->leftPressed) {
            //TileMap will add a tile to its own tiles
            edittingTileMap->AddTileToMap(mouseX, mouseY, cameraX, cameraY, WindowTileMapBrush::GetInstance().selectedTile);
        }
        if (SceneManager::GetInstance().openedScene->deletePressed) {
            //TileMap will remove the tile from its own tiles
            edittingTileMap->RemoveTileFromMap(mouseX, mouseY, cameraX, cameraY);
        }
    }

	ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
INTERFACEMANAGER_API WindowTileMapViewer& WindowTileMapViewer::GetInstance()
{
	static WindowTileMapViewer window;
	return window;
}
