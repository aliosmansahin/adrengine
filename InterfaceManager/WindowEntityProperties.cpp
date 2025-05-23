#include "pch.h"
#include "WindowEntityProperties.h"
#include "Localization.h"
#include "VisualScriptManager.h"
#include "SceneManager.h"

/*
PURPOSE: Draws the window
*/
void WindowEntityProperties::DrawWindow(
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::string& selectedTabId)
{
    //Begins the window
    ImGui::Begin(Localization::GetString("interface_main_menu_item_entity_properties"), &showWindow);
    ImGui::SetWindowFontScale(1.5f);

    //Some variables
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float padding = 4 * ImGui::GetStyle().ItemSpacing.x;
    float itemWidth = (totalWidth - 2 * spacing) / 3.0f;

    //If an entity is selected
    if (currentEntity) {
        //store entity id
        std::string idStr = currentEntity->GetEntityParams()->name;

        //Buffer for the id input
        static char idBuf[32] = "";

        //If the entity id changed
        if (idBuf != idStr) { 
            strncpy_s(idBuf, idStr.c_str(), sizeof(idBuf));
            idBuf[sizeof(idBuf) - 1] = '\0';
        }

        //Get new id
        if (ImGui::InputText("Entity Id", idBuf, sizeof(idBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            //If the input is empty
            if (idBuf[0] == '\0') {
                strncpy_s(idBuf, currentEntity->GetEntityParams()->name.c_str(), sizeof(idBuf));
                idBuf[sizeof(idBuf) - 1] = '\0';
            }
            //Otherwise change the entity id
            else {
                std::string oldName = currentEntity->GetEntityParams()->name;
                currentEntity->GetEntityParams()->name = std::string(idBuf);
            }
        }
        
        //Position drags
        ImGui::SeparatorText("Position");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pX", &currentEntity->GetEntityParams()->x);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pY", &currentEntity->GetEntityParams()->y);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pZ", &currentEntity->GetEntityParams()->z);

        //Rotation drags
        ImGui::SeparatorText("Rotation");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rX", &currentEntity->GetEntityParams()->rx);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rY", &currentEntity->GetEntityParams()->ry);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rZ", &currentEntity->GetEntityParams()->rz);

        //Scale drags
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

        //Script selections
        ImGui::SeparatorText("Script");
        //If this entity has no script, draw an add button
        if (currentEntity->GetEntityParams()->scriptId.empty()) {
            ImGui::Text("No Script");
            ImGui::SameLine();
            if (ImGui::Button("Add Script")) {
                ScriptBelongsTo sbt;
                sbt.entity = currentEntity;
                sbt.sceneJson = SceneManager::GetInstance().currentScene->ToJson();
                VisualScriptManager::GetInstance().CreateScript(sbt, projectDir, tabs, openedTab, selectedTabId, SceneManager::GetInstance().scenes);
                selectedTabId = currentEntity->GetEntityParams()->scriptId;
            }
        }
        //Otherwise draw an edit button for the script
        else {
            ImGui::Text(currentEntity->GetEntityParams()->scriptId.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Edit Script")) {
                auto result = VisualScriptManager::GetInstance().OpenScript(currentEntity->GetEntityParams()->scriptId, tabs);
                VisualScriptManager::GetInstance().currentScript = result.first;
                openedTab = result.second.get();
                selectedTabId = result.second->id;
            }
        }

        /*
        TODO: For now, draw each property of each entity types
            THIS WILL BE MOVED TO OTHERWHERE

            COMMENTS WILL BE ADDED
        */
        if (currentEntity->GetEntityParams()->GetType() == "Sprite2D") {
            ImGui::Separator();
            ImGui::SeparatorText("Texture");

            //Cast to the params of sprite2d
            auto casted = dynamic_cast<Sprite2DParams*>(currentEntity->GetEntityParams());
            if (casted) {
                //Store entity id
                std::string textureIdStr = casted->textureId;

                static char textureIdBuf[32] = "";

                if (textureIdBuf != textureIdStr) { // if the entity id changed
                    strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
                    textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                }

                if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) { //CHANGE IT TO OPTION PANEL THAT CONTAINS ALL TEXTURES
                    if (textureIdBuf[0] == '\0') {// if the input is empty
                        strncpy_s(textureIdBuf, currentEntity->GetEntityParams()->name.c_str(), sizeof(textureIdBuf));
                        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                    }
                    else {
                        if (AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf)).get()) {
                            casted->textureId = std::string(textureIdBuf);
                            casted->texture = AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf))->texture;
                        }
                    }
                }

                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ID")) {
                        const char* payloadData = static_cast<const char*>(payload->Data);
                        std::string droppedId(payloadData, payload->DataSize);

                        auto& textures = AssetDatabase::GetInstance().GetTextures();
                        auto textureIter = textures.find(droppedId);
                        
                        if (textureIter != textures.end()) {
                            auto& texture = textureIter->second;
                            if (texture.get()) {
                                casted->textureId = texture->id;
                                casted->texture = texture->texture;
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "Object") {
            ImGui::Separator();
            ImGui::SeparatorText("Mesh");
            auto casted = dynamic_cast<ObjectParams*>(currentEntity->GetEntityParams());
            if (casted) {
                std::string objIdStr = casted->objId; // store entity id

                static char objIdBuf[32] = "";

                if (objIdBuf != objIdStr) { // if the entity id changed
                    strncpy_s(objIdBuf, objIdStr.c_str(), sizeof(objIdBuf));
                    objIdBuf[sizeof(objIdBuf) - 1] = '\0';
                }

                if (ImGui::InputText("Mesh Id", objIdBuf, sizeof(objIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (objIdBuf[0] == '\0') {// if the input is empty
                        strncpy_s(objIdBuf, currentEntity->GetEntityParams()->name.c_str(), sizeof(objIdBuf));
                        objIdBuf[sizeof(objIdBuf) - 1] = '\0';
                    }
                    else {
                        auto mesh = AssetDatabase::GetInstance().GetMesh(std::string(objIdBuf)).get();
                        if (mesh) {
                            casted->objId = mesh->id;
                            casted->objVAO = mesh->meshVAO;
                            casted->objVerticeCount = mesh->verticeCount;
                        }
                    }
                }

                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_ID")) {
                        const char* payloadData = static_cast<const char*>(payload->Data);
                        std::string droppedId(payloadData, payload->DataSize);

                        auto& meshes = AssetDatabase::GetInstance().GetMeshes();
                        auto meshIter = meshes.find(droppedId);

                        if (meshIter != meshes.end()) {
                            auto& mesh = meshIter->second;
                            if (mesh.get()) {
                                casted->objId = mesh->id;
                                casted->objVAO = mesh->meshVAO;
                                casted->objVerticeCount = mesh->verticeCount;
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "DirectionalLight") {
            auto casted = dynamic_cast<DirectionalLightParams*>(currentEntity->GetEntityParams());
            if (casted) {
                ImGui::Separator();
                ImGui::SeparatorText("Direction");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("X", &casted->direction.x);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Y", &casted->direction.y);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Z", &casted->direction.z);
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "SpotLight") {
            auto casted = dynamic_cast<SpotLightParams*>(currentEntity->GetEntityParams());
            if (casted) {
                ImGui::Separator();
                ImGui::SeparatorText("Direction");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("X", &casted->direction.x, 0.01f);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Y", &casted->direction.y, 0.01f);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Z", &casted->direction.z, 0.01f);

                ImGui::Separator();
                ImGui::SeparatorText("Calculation");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("constant", &casted->constant, 0.01f);
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("linear", &casted->linear, 0.01f);
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("quadratic", &casted->quadratic, 0.01f);

                ImGui::Separator();
                ImGui::SeparatorText("Cones");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("outer", &casted->outerCutOff, 0.01f);
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("inner", &casted->cutOff, 0.01f);

            }
        }

        else if (currentEntity->GetEntityParams()->GetType() == "PointLight") {
            auto casted = dynamic_cast<PointLightParams*>(currentEntity->GetEntityParams());
            if (casted) {
                ImGui::Separator();
                ImGui::SeparatorText("Calculation");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("constant", &casted->constant, 0.01f);
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("linear", &casted->linear, 0.01f);
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("quadratic", &casted->quadratic, 0.01f);
            }
            }
    }
    else {
        ImGui::TextColored(ImVec4(0, 255, 0, 255), "Select an entity to modify");
    }
    ImGui::End();
}

/*
PURPOSE: Change the currentEntity to the selected one
*/
void WindowEntityProperties::SelectEntity(Entity* entity)
{
    currentEntity = entity;
}

/*
PURPOSE: Gets the instance of the class
*/
WindowEntityProperties& WindowEntityProperties::GetInstance()
{
    static WindowEntityProperties window;
    return window;
}
