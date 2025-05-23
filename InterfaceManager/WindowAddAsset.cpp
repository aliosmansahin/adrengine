#include "pch.h"
#include "WindowAddAsset.h"
#include "SceneManager.h"
#include "AssetDatabase.h"

/*
PURPOSE: Draws window
*/
void WindowAddAsset::DrawWindow(std::string& projectDir, std::string& assetExplorerType)
{
    //Begin the window
    ImGui::Begin("Add New Asset", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
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
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("Asset Path", "Path", buf, sizeof(buf));
    static std::string path = "";
    if (ImGui::IsItemEdited()) {
        buf[sizeof(buf) - 1] = '\0';
        path = buf;
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        //TODO: Implement it
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
            AssetDatabase::GetInstance().SaveDatabase(projectDir + "asset_database.adrenginedatabase");
            memset(buf, 0, sizeof(buf));
            memset(nameBuf, 0, sizeof(nameBuf));
            showWindow = false;
        }
    }
    ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAddAsset& WindowAddAsset::GetInstance()
{
    static WindowAddAsset window;
    return window;
}
