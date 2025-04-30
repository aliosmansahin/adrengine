#include "WindowEntityProperties.h"

void WindowEntityProperties::DrawWindow()
{
    ImGui::Begin(Localization::Get().GetString("interface_main_menu_item_entity_properties"), &showWindow);
    ImGui::SetWindowFontScale(1.5f);
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float padding = 4 * ImGui::GetStyle().ItemSpacing.x;
    float itemWidth = (totalWidth - 2 * spacing) / 3.0f;

    if (currentEntity) {
        //id input
        std::string idStr = currentEntity->GetEntityParams()->name; // store entity id

        static char idBuf[32] = "";

        if (idBuf != idStr) { // if the entity id changed
            strncpy_s(idBuf, idStr.c_str(), sizeof(idBuf));
            idBuf[sizeof(idBuf) - 1] = '\0';
        }
        // get new id
        if (ImGui::InputText("Entity Id", idBuf, sizeof(idBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (idBuf[0] == '\0') {// if the input is empty
                strncpy_s(idBuf, currentEntity->GetEntityParams()->name.c_str(), sizeof(idBuf));
                idBuf[sizeof(idBuf) - 1] = '\0';
            }
            else {//otherwise change the entity id
                std::string oldName = currentEntity->GetEntityParams()->name;
                currentEntity->GetEntityParams()->name = std::string(idBuf);
            }
        }
        
        //position drags
        ImGui::SeparatorText("Position");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pX", &currentEntity->GetEntityParams()->x);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pY", &currentEntity->GetEntityParams()->y);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pZ", &currentEntity->GetEntityParams()->z);

        //rotation drags
        ImGui::SeparatorText("Rotation");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rX", &currentEntity->GetEntityParams()->rx);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rY", &currentEntity->GetEntityParams()->ry);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rZ", &currentEntity->GetEntityParams()->rz);

        //scale drags
        float scaleDragSpeed = 0.01f;
        ImGui::SeparatorText("Scale");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sX", &currentEntity->GetEntityParams()->sx, scaleDragSpeed);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sY", &currentEntity->GetEntityParams()->sy, scaleDragSpeed);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sZ", &currentEntity->GetEntityParams()->sz, scaleDragSpeed);

        ImGui::Separator();

        ImGui::SeparatorText("Script");
        if (currentEntity->GetEntityParams()->scriptId.empty()) {
            ImGui::Text("No Script");
            ImGui::SameLine();
            if (ImGui::Button("Add Script")) {
                ScriptBelongsTo sbt;
                sbt.entity = currentEntity;
                sbt.scene = SceneManager::GetInstance().currentScene;
                VisualScriptManager::GetInstance().CreateScript(sbt);
                InterfaceManager::GetInstance().selectedTabId = currentEntity->GetEntityParams()->scriptId;
            }
        }
        else {
            ImGui::Text(currentEntity->GetEntityParams()->scriptId.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Edit Script")) {
                auto result = VisualScriptManager::GetInstance().OpenScript(currentEntity->GetEntityParams()->scriptId);
                InterfaceManager::GetInstance().selectedTabId = currentEntity->GetEntityParams()->scriptId;
                VisualScriptManager::GetInstance().currentScript = result.first;
                InterfaceManager::GetInstance().openedTab = result.second.get();
                InterfaceManager::GetInstance().selectedTabId = result.second->id;
            }
        }

        if (currentEntity->GetEntityParams()->GetType() == "Sprite2D") {
            ImGui::Separator();
            ImGui::SeparatorText("Texture");
            auto casted = dynamic_cast<Sprite2DParams*>(currentEntity->GetEntityParams());
            if (casted) {
                std::string textureIdStr = casted->textureId; // store entity id

                static char textureIdBuf[32] = "";

                if (textureIdBuf != textureIdStr) { // if the entity id changed
                    strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
                    textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                }

                if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (textureIdBuf[0] == '\0') {// if the input is empty
                        strncpy_s(textureIdBuf, currentEntity->GetEntityParams()->name.c_str(), sizeof(textureIdBuf));
                        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                    }
                    else {//otherwise change the entity id
                        casted->textureId = std::string(textureIdBuf);
                        casted->texture = Graphics::GetInstance().GetTexture(std::string(textureIdBuf));
                    }
                }
            }
        }
    }
    else {
        ImGui::TextColored(ImVec4(0, 255, 0, 255), "Select an entity to modify");
    }
    ImGui::End();
}

void WindowEntityProperties::SelectEntity(Entity* entity)
{
    currentEntity = entity;
}

WindowEntityProperties& WindowEntityProperties::GetInstance()
{
    static WindowEntityProperties window;
    return window;
}
