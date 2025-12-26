#include "pch.h"
#include "WindowAddAsset.h"
#include "AssetDatabase.h"

#include "interfaces/IEngine/IEngine.h"

/*
PURPOSE: Draws window
*/
void WindowAddAsset::DrawWindow(std::string& assetExplorerType)
{
    float posX = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().first - defSize.x) / 2.0f;
    float posY = (ServiceLocator::Get<IEngine>()->GetMainWindowSize().second - defSize.y) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(defSize, ImGuiCond_FirstUseEver);

    //Begin the window
    ImGui::Begin(GetWindowTitleWithID().c_str(), &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    //The combobox of asset types
    if(ImGui::BeginCombo("Asset Type", assetExplorerType.c_str())) {
        if (ImGui::Selectable("Textures")) {
            assetExplorerType = "Textures";
        }
        if (ImGui::Selectable("Meshes")) {
            assetExplorerType = "Meshes";
        }
        ImGui::EndCombo();
    }

    //Inputbox of path of asset
    ImGui::SetNextItemWidth(500.0f);
    ImGui::InputTextWithHint("Asset Path", "Path", buf, sizeof(buf));
    static std::string path = "";
    if (ImGui::IsItemEdited()) {
        buf[sizeof(buf) - 1] = '\0';
        path = buf;
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        config.countSelectionMax = 1;
        ImGuiFileDialog::Instance()->OpenDialog("#window_add_asset_file_dialog", "Select a file", ".obj", config);
    }

    //Inputbox of asset name
    static char nameBuf[32] = { 0 };
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("Asset Name", "Name", nameBuf, sizeof(nameBuf));
    static std::string assetName = "";
    if (ImGui::IsItemEdited()) {
        nameBuf[sizeof(nameBuf) - 1] = '\0';
        assetName = nameBuf;
    }

    //Add the asset as the type of it
    if (!path.empty()) {
        if (ImGui::Button("Add")) {
            //asset type is texture, so create a texture
            if (assetExplorerType == "Textures") {
                AssetDatabase::GetInstance().CreateTexture(path, assetName);
            }
            //asset type is mesh, so create a mesh
            else if (assetExplorerType == "Meshes") {
                AssetDatabase::GetInstance().CreateMesh(path, assetName);
            }

            //after that save all database and close the window
            //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
            AssetDatabase::GetInstance().SaveDatabase(ServiceLocator::Get<IProject>()->GetProjectDir() + "asset_database.adrenginedatabase");
            memset(buf, 0, sizeof(buf));
            memset(nameBuf, 0, sizeof(nameBuf));
            showWindow = false;
        }
    }
    ImGui::End();

    //Open file dialog
    if (ImGuiFileDialog::Instance()->Display("#window_add_asset_file_dialog", ImGuiWindowFlags_NoDocking, ImVec2(700.0f, 500.0f))) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            //Get the file path with file name
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();

            strcpy_s(buf, sizeof(buf), filePath.c_str());
            path = filePath;
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAddAsset& WindowAddAsset::GetInstance()
{
    static WindowAddAsset window;
    return window;
}
