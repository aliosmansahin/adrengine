#include "pch.h"
#include "WindowEntityProperties.h"
#include "Localization.h"
#include "VisualScriptManager.h"
#include "SceneManager.h"

#include "WindowGameViewport.h"

/*
PURPOSE: Draws the window
*/
void WindowEntityProperties::DrawWindow(
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs,
    Utils::Tab*& openedTab,
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
        
        glm::vec3 entityPosition = currentEntity->GetEntityParams()->GetPosition();

        //Position drags
        ImGui::SeparatorText("Position");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pX", &entityPosition.x, 0.1f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pY", &entityPosition.y, 0.1f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("pZ", &entityPosition.z, 0.1f);

        if(!WindowGameViewport::GetInstance().isPlaying)
            currentEntity->GetEntityParams()->SetEditorPosition(entityPosition);


        glm::vec3 entityRotation = currentEntity->GetEntityParams()->GetRotation();

        //Rotation drags
        ImGui::SeparatorText("Rotation");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rX", &entityRotation.x);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rY", &entityRotation.y);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("rZ", &entityRotation.z);

        if (!WindowGameViewport::GetInstance().isPlaying)
            currentEntity->GetEntityParams()->SetEditorRotation(entityRotation);


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
        if (!currentEntity->GetEntityParams()->script) {
            ImGui::Text("No Script");
            ImGui::SameLine();
            if (ImGui::Button("Add Script")) {
                Utils::ScriptBelongsTo sbt;
                sbt.entityJson = currentEntity->ToJson();
                sbt.sceneJson = SceneManager::GetInstance().openedScene->ToJson();
                currentEntity->GetEntityParams()->script = VisualScriptManager::GetInstance().CreateScript(sbt, projectDir, tabs, openedTab, selectedTabId, SceneManager::GetInstance().scenes);

                sbt.entityJson["scriptId"] = currentEntity->GetEntityParams()->script->scriptId;

                AssetSaver::SaveEntityToFile(sbt.entityJson, projectDir, currentEntity->GetEntityParams()->id);
            }
        }
        //Otherwise draw an edit button for the script
        else {
            ImGui::Text(currentEntity->GetEntityParams()->script->scriptId.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Edit Script")) {
                auto result = VisualScriptManager::GetInstance().OpenScript(currentEntity->GetEntityParams()->script, tabs);
                openedTab = result.second.get();
                selectedTabId = result.second->id;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Script")) {
                VisualScriptManager::GetInstance().DeleteScript(currentEntity->GetEntityParams()->script.get(), tabs, openedTab, projectDir);

                currentEntity->GetEntityParams()->script = nullptr;

                AssetSaver::SaveEntityToFile(currentEntity->ToJson(), projectDir, currentEntity->GetEntityParams()->id);
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
            auto object = dynamic_cast<Object*>(currentEntity);
            auto casted = dynamic_cast<ObjectParams*>(currentEntity->GetEntityParams());
            if (object && casted) {
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
                            casted->mesh = mesh;
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
                                casted->mesh = mesh.get();
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                //PHYSICS
                ImGui::Separator();
                ImGui::SeparatorText("Physics");

                //Get properties from the rigidbody
                Physics* physics = SceneManager::GetInstance().openedScene->physics;
                RigidBodyProperties* props = object->rigidBody->GetProps();

                //Update properties
                ImGui::Checkbox("Is Kinematic", &props->isKinematic);
                ImGui::DragFloat("Mass", &props->mass, 0.01f);
                ImGui::DragFloat("Restitution", &props->restitution, 0.01f);
                ImGui::DragFloat("Linear Damping", &props->linearDamping, 0.01f);
                ImGui::DragFloat("Angular Damping", &props->angularDamping, 0.01f);

                if (object->rigidBody->GetShape() == RigidBodyShape::Box) {
                    glm::vec3& half = object->rigidBody->GetProps()->shapeProps.halfExtentsForBox;
                    ImGui::SeparatorText("Half Extents");
                    ImGui::SetNextItemWidth(itemWidth - padding);
                    ImGui::DragFloat("X", &half.x, 0.1f, 0.0f, 10000.0f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(itemWidth - padding);
                    ImGui::DragFloat("Y", &half.y, 0.1f, 0.0f, 10000.0f);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(itemWidth - padding);
                    ImGui::DragFloat("Z", &half.z, 0.1f, 0.0f, 10000.0f);
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
        else if (currentEntity->GetEntityParams()->GetType() == "TileMap") {
            auto casted = dynamic_cast<TileMapParams*>(currentEntity->GetEntityParams());
            if (casted) {
                TileMap* tileMap = dynamic_cast<TileMap*>(currentEntity);
                
                ImGui::Separator();
                ImGui::SeparatorText("TileMap");
                if (ImGui::Button("Edit this TileMap")) {
                    WindowTileMapEdit::GetInstance().showWindow = true;
                    WindowTileMapEdit::GetInstance().editingTileMap = tileMap;
                }
                
                ImGui::Separator();
                if (tileMap->GetCreatedTiles().empty()) {
                    ImGui::Text("No tiles found,\n\tto use brush tool,\n\tcreate tiles with\n\t\"Edit Tilemap\" tool");
                }
                else {
                    if (ImGui::Button("Tile map brush")) {
                        WindowTileMapBrush::GetInstance().showWindow = true;
                        WindowTileMapBrush::GetInstance().editingTileMap = tileMap;
                    }
                }
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "FlipBook") {
            auto casted = dynamic_cast<FlipBookParams*>(currentEntity->GetEntityParams());
            if (casted) {
                FlipBook* flipBook = dynamic_cast<FlipBook*>(currentEntity);

                ImGui::Separator();
                ImGui::SeparatorText("FlipBook");

                if (ImGui::Button("Edit this FlipBook")) {
                    WindowFlipBookEdit::GetInstance().showWindow = true;
                    WindowFlipBookEdit::GetInstance().edittingFlipBook = flipBook;
                }

                if (!flipBook->GetFrames().empty()) {
                    ImGui::DragFloat("Frame Duration", &casted->frameWait, 0.001f, 0.0f);
                    ImGui::Checkbox("Loop", &casted->loop);
                    if (ImGui::Button("Restart FlipBook"))
                        flipBook->RestartFlipBook();
                }
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "Camera") {
            auto casted = dynamic_cast<CameraParams*>(currentEntity->GetEntityParams());
            if (casted) {
                Camera* flipBook = dynamic_cast<Camera*>(currentEntity);

                ImGui::Separator();
                ImGui::SeparatorText("Projection");

                ImGui::Text("Projection Type"); ImGui::SameLine();

                //Selection for the type of the scene
                ImGui::RadioButton("2D", &(int&)casted->projectionType, (int)CameraProjection::ORTHOGRAPHIC); ImGui::SameLine();
                ImGui::RadioButton("3D", &(int&)casted->projectionType, (int)CameraProjection::PERPECTIVE);

                if (casted->projectionType == CameraProjection::ORTHOGRAPHIC) {
                    ImGui::Separator();
                    ImGui::SeparatorText("Orthographic");
                }
                else if(casted->projectionType == CameraProjection::PERPECTIVE) {
                    ImGui::Separator();
                    ImGui::SeparatorText("Perspective");

                    ImGui::DragFloat("FOV", &casted->fov, 1.0f, 10.0f, 170.0f);
                }
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
