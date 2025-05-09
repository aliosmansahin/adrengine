#include "WindowAddAsset.h"

void WindowAddAsset::DrawWindow()
{
    ImGui::Begin("Add New Asset", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("Asset Path", "Path", buf, sizeof(buf));
    static std::string path = "";
    if (ImGui::IsItemEdited()) {
        buf[sizeof(buf) - 1] = '\0';
        path = buf;
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        //open file dialog
    }

    static char nameBuf[32] = { 0 };
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("Asset Name", "Name", nameBuf, sizeof(nameBuf));
    static std::string assetName = "";
    if (ImGui::IsItemEdited()) {
        nameBuf[sizeof(nameBuf) - 1] = '\0';
        assetName = nameBuf;
    }

    if (!path.empty()) {
        if (ImGui::Button("Add")) {
            AssetDatabase::GetInstance().CreateTexture(path, assetName);

            std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
            AssetDatabase::GetInstance().SaveDatabase(projectDir + "asset_database.adrenginedatabase");
            memset(buf, 0, sizeof(buf));
            memset(nameBuf, 0, sizeof(nameBuf));
            showWindow = false;
        }
    }
    ImGui::End();
}

WindowAddAsset& WindowAddAsset::GetInstance()
{
    static WindowAddAsset window;
    return window;
}
