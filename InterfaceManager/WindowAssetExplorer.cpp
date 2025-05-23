#include "pch.h"
#include "WindowAssetExplorer.h"

/*
PURPOSE: Draws the window
*/
void WindowAssetExplorer::DrawWindow()
{
    //Begin the window
    ImGui::Begin("Asset Explorer", &showWindow);
    ImGui::SetWindowFontScale(1.5f);

    //Create New One button
    if (ImGui::Button("Create New One")) {
        WindowAddAsset::GetInstance().showWindow = true;
    }

    ImGui::SameLine();

    //To select asset type to show
    if (ImGui::BeginCombo("Asset Type", currentSelection.c_str())) {
        if (ImGui::Selectable("Textures"))
            currentSelection = "Textures";
        if (ImGui::Selectable("Meshes"))
            currentSelection = "Meshes";
        ImGui::EndCombo();
    }

    //Show the assets
    if (currentSelection == "Textures")
        DrawTexturesPage();
    else if (currentSelection == "Meshes")
        DrawMeshesPage();
    
    //End the window
    ImGui::End();
}

/*
PURPOSE: Draws texture assets
*/
INTERFACEMANAGER_API void WindowAssetExplorer::DrawTexturesPage()
{
    //Draw a title
    ImGui::SeparatorText("Textures");

    //Draw each texture asset
    auto& database = AssetDatabase::GetInstance().GetTextures();
    for (auto& iter : database) {
        bool selected = false;

        //Selectable for the asset
        ImGui::Selectable(iter.second->name.c_str(), &selected, ImGuiSelectableFlags_None, ImVec2(100, 100));

        //TODO: Make image button

        //This selectable will be source for dragging
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("TEXTURE_ID", iter.second->id.c_str(), iter.second->id.size());
            ImGui::Text("Drop", iter.second->id);
            ImGui::EndDragDropSource();
        }

        //When user right-clicks, open a popup
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            selectedId = iter.second->id;
            ImGui::OpenPopup("TexturePopup");
        }
        ImGui::SameLine();
    }

    //Draw the popup
    if (ImGui::BeginPopup("TexturePopup")) {
        if (ImGui::MenuItem("Delete")) {
            //Delete the texture, after that, selected asset will be nothing
            AssetDatabase::GetInstance().DeleteTexture(selectedId);
            selectedId = "";
        }
        ImGui::EndPopup();
    }
}

/*
PURPOSE: Draws mesh assets
*/
INTERFACEMANAGER_API void WindowAssetExplorer::DrawMeshesPage()
{
    //Draw a title
    ImGui::SeparatorText("Meshes");

    //Draw each mesh asset
    auto& database = AssetDatabase::GetInstance().GetMeshes();
    for (auto& iter : database) {
        bool selected = false;

        //Selectable for the asset
        ImGui::Selectable(iter.second->name.c_str(), &selected, ImGuiSelectableFlags_None, ImVec2(100, 100));
        
        //TODO: Make image button

        //This selectable will be source for dragging
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("MESH_ID", iter.second->id.c_str(), iter.second->id.size());
            ImGui::Text("Drop", iter.second->id);
            ImGui::EndDragDropSource();
        }

        //When user right-clicks, open a popup
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            selectedId = iter.second->id;
            ImGui::OpenPopup("MeshPopup");
        }
        ImGui::SameLine();
    }

    //Draw the popup
    if (ImGui::BeginPopup("MeshPopup")) {
        if (ImGui::MenuItem("Delete")) {
            //Delete the mesh, after that, selected asset will be nothing
            AssetDatabase::GetInstance().DeleteMesh(selectedId);
            selectedId = "";
        }
        ImGui::EndPopup();
    }
}

/*
PURPOSE: Gets the instance of the class
*/
WindowAssetExplorer& WindowAssetExplorer::GetInstance()
{
    static WindowAssetExplorer window;
    return window;
}
