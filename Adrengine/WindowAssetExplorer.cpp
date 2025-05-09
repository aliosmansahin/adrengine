#include "WindowAssetExplorer.h"

void WindowAssetExplorer::DrawWindow()
{
    ImGui::Begin("Asset Explorer", &showWindow);
    ImGui::SetWindowFontScale(1.5f);
    if (ImGui::Button("Create New One")) {
        WindowAddAsset::GetInstance().showWindow = true;
    }

    ImGui::SeparatorText("Textures");

    auto& database = AssetDatabase::GetInstance().GetTextures();
    for (auto& iter : database) {
        bool selected = false;
        ImGui::Selectable(iter.second->name.c_str(), &selected, ImGuiSelectableFlags_None, ImVec2(100, 100));
        //make image button

        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("TEXTURE_ID", iter.second->id.c_str(), iter.second->id.size());
            ImGui::Text("Drop", iter.second->id);
            ImGui::EndDragDropSource();
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            selectedId = iter.second->id;
            ImGui::OpenPopup("TexturePopup");
        }
        ImGui::SameLine();
    }

    if (ImGui::BeginPopup("TexturePopup")) {
        if (ImGui::MenuItem("Delete")) {
            AssetDatabase::GetInstance().DeleteTexture(selectedId);
            selectedId = "";
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

WindowAssetExplorer& WindowAssetExplorer::GetInstance()
{
    static WindowAssetExplorer window;
    return window;
}
