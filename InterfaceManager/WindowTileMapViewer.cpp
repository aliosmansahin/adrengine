#include "pch.h"
#include "WindowTileMapViewer.h"

INTERFACEMANAGER_API void WindowTileMapViewer::DrawWindow()
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

    Graphics::GetInstance().BindFramebuffer();
    Graphics::GetInstance().Clear();
    adr_glViewport(0, 0, window_width, window_height);





	ImGui::End();
}

INTERFACEMANAGER_API WindowTileMapViewer& WindowTileMapViewer::GetInstance()
{
	static WindowTileMapViewer window;
	return window;
}
