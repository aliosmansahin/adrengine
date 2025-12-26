#include "pch.h"
#include "WindowEntityProperties.h"
#include "Localization.h"

#include "WindowGameViewport.h"
#include "WindowTileMapEdit.h"
#include "WindowTileMapBrush.h"
#include "WindowFlipBookEdit.h"

#include "InterfaceManager.h"

/*
PURPOSE: Draws the window
*/
void WindowEntityProperties::DrawWindow()
{
    //Getter for tabs
	auto& tabs = InterfaceManager::GetInstance().GetTabs();
	auto openedTab = InterfaceManager::GetInstance().GetOpenedTab();
	auto selectedTabId = InterfaceManager::GetInstance().GetSelectedTabId();

    //Begins the window
    ImGui::Begin(GetWindowTitleWithID().c_str(), &showWindow);
    ImGui::SetWindowFontScale(1.5f);

    //Some variables
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float padding = 4 * ImGui::GetStyle().ItemSpacing.x;
    float itemWidth = (totalWidth - 2 * spacing) / 3.0f;

    //If an entity is selected
    if (currentEntity) {
        //store entity id
        std::string idStr = currentEntity->GetEntityParams()->GetName();

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
                strncpy_s(idBuf, currentEntity->GetEntityParams()->GetName().c_str(), sizeof(idBuf));
                idBuf[sizeof(idBuf) - 1] = '\0';
            }
            //Otherwise change the entity id
            else {
                std::string oldName = currentEntity->GetEntityParams()->GetName();
                currentEntity->GetEntityParams()->SetName(std::string(idBuf));
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

        glm::vec3 entityScale = currentEntity->GetEntityParams()->GetScale();

        //Scale drags
        float scaleDragSpeed = 0.01f;
        ImGui::SeparatorText("Scale");
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sX", &entityScale.x, scaleDragSpeed);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sY", &entityScale.y, scaleDragSpeed);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(itemWidth - padding);
        ImGui::DragFloat("sZ", &entityScale.z, scaleDragSpeed);

        if (!WindowGameViewport::GetInstance().isPlaying)
            currentEntity->GetEntityParams()->SetEditorScale(entityScale);

        ImGui::Separator();

        //Script selections
        ImGui::SeparatorText("Script");
        //If this entity has no script, draw an add button
        if (!currentEntity->GetEntityParams()->GetVisualScript()) {
            ImGui::Text("No Script");
            ImGui::SameLine();
            if (ImGui::Button("Add Script")) {
                Utils::ScriptBelongsTo sbt;
                sbt.entityJson = currentEntity->ToJson();
                sbt.sceneJson = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->ToJson();
                std::shared_ptr<IVisualScript> script = ServiceLocator::Get<IVisualScriptManager>()->CreateScript(sbt);

                if (script) {
					//Setup script for the entity
                    currentEntity->GetEntityParams()->SetVisualScript(script);

                    sbt.entityJson["scriptId"] = currentEntity->GetEntityParams()->GetVisualScript()->GetScriptId();

                    AssetSaver::SaveEntityToFile(sbt.entityJson, ServiceLocator::Get<IProject>()->GetProjectDir(), currentEntity->GetEntityParams()->GetId());

					//Open a new tab for the script
					InterfaceManager::GetInstance().AddTab(script->GetScriptId(), Utils::TabType::VisualScriptEditor);
					InterfaceManager::GetInstance().ActivateTab(script->GetScriptId());
                }

            }
        }
        //Otherwise draw an edit button for the script
        else {
            ImGui::Text(currentEntity->GetEntityParams()->GetVisualScript()->GetScriptId().c_str());
            ImGui::SameLine();
            if (ImGui::Button("Edit Script")) {
                auto script = ServiceLocator::Get<IVisualScriptManager>()->OpenScript(currentEntity->GetEntityParams()->GetVisualScript());
                if (script) {
					//Create a new tab if not exist
                    if(!InterfaceManager::GetInstance().GetTabById(script->GetScriptId()))
                        InterfaceManager::GetInstance().AddTab(script->GetScriptId(), Utils::TabType::VisualScriptEditor);

					//Activate the tab
					InterfaceManager::GetInstance().ActivateTab(script->GetScriptId());
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Script")) {
				auto script = currentEntity->GetEntityParams()->GetVisualScript();

                if (ServiceLocator::Get<IVisualScriptManager>()->DeleteScript(script)) {
				    std::string scriptId = script->GetScriptId();

					//Remove tab if exist
                    if(InterfaceManager::GetInstance().GetTabById(scriptId))
                        InterfaceManager::GetInstance().RemoveTab(scriptId);

					//Remove script from the entity
                    currentEntity->GetEntityParams()->SetVisualScript(nullptr);

                    AssetSaver::SaveEntityToFile(currentEntity->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), currentEntity->GetEntityParams()->GetId());
                }
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
            auto casted = std::dynamic_pointer_cast<ISprite2DParams>(currentEntity->GetEntityParams());
            if (casted) {
                //Store entity id
                std::string textureIdStr = casted->GetTextureId();

                static char textureIdBuf[32] = "";

                if (textureIdBuf != textureIdStr) { // if the entity id changed
                    strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
                    textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                }

                if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) { //CHANGE IT TO OPTION PANEL THAT CONTAINS ALL TEXTURES
                    if (textureIdBuf[0] == '\0') {// if the input is empty
                        strncpy_s(textureIdBuf, currentEntity->GetEntityParams()->GetName().c_str(), sizeof(textureIdBuf));
                        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
                    }
                    else {
                        if (AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf)).get()) {
                            casted->SetTextureId(std::string(textureIdBuf));
                            casted->SetTexture(AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf))->texture);
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
                                casted->SetTextureId(texture->id);
                                casted->SetTexture(texture->texture);
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
            auto object = std::dynamic_pointer_cast<IObject>(currentEntity);
            auto casted = std::dynamic_pointer_cast<IObjectParams>(currentEntity->GetEntityParams());
            if (object && casted) {
                std::string objIdStr = casted->GetObjId(); // store obj id

                static char objIdBuf[32] = "";

                if (objIdBuf != objIdStr) { // if the obj id changed
                    strncpy_s(objIdBuf, objIdStr.c_str(), sizeof(objIdBuf));
                    objIdBuf[sizeof(objIdBuf) - 1] = '\0';
                }

                if (ImGui::InputText("Mesh Id", objIdBuf, sizeof(objIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (objIdBuf[0] == '\0') {// if the input is empty
                        strncpy_s(objIdBuf, currentEntity->GetEntityParams()->GetName().c_str(), sizeof(objIdBuf));
                        objIdBuf[sizeof(objIdBuf) - 1] = '\0';
                    }
                    else {
                        auto mesh = AssetDatabase::GetInstance().GetMesh(std::string(objIdBuf));
                        if (mesh) {
                            casted->SetObjId(mesh->id);
                            casted->SetMesh(mesh);
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
                            if (mesh) {
                                casted->SetObjId(mesh->id);
                                casted->SetMesh(mesh);
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                //PHYSICS
                ImGui::Separator();
                ImGui::SeparatorText("Physics");

                //Get properties from the rigidbody
                auto sceneManager = ServiceLocator::Get<ISceneManager>();
				std::shared_ptr<IScene> scene = sceneManager->GetOpenedScene();
                std::shared_ptr<IPhysics> physics = scene->GetPhysics();
                std::shared_ptr<IRigidBody> rigidBody = object->GetRigidBody();
                std::shared_ptr<RigidBodyProperties> props = rigidBody->GetProps();

                if (props) {
                    //Update properties
                    ImGui::Checkbox("Is Kinematic", &props->isKinematic);
                    ImGui::DragFloat("Mass", &props->mass, 0.01f);
                    ImGui::DragFloat("Restitution", &props->restitution, 0.01f);
                    ImGui::DragFloat("Linear Damping", &props->linearDamping, 0.01f);
                    ImGui::DragFloat("Angular Damping", &props->angularDamping, 0.01f);

                    if (object->GetRigidBody()->GetShape() == RigidBodyShape::Box) {
                        glm::vec3& half = object->GetRigidBody()->GetProps()->shapeProps.halfExtentsForBox;
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
        }
        else if (currentEntity->GetEntityParams()->GetType() == "DirectionalLight") {
            auto casted = std::dynamic_pointer_cast<IDirectionalLightParams>(currentEntity->GetEntityParams());
            if (casted) {
                glm::vec3 lightDirection = casted->GetDirection();

                ImGui::Separator();
                ImGui::SeparatorText("Direction");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("X", &lightDirection.x);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Y", &lightDirection.y);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Z", &lightDirection.z);

                casted->SetDirection(lightDirection);
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "SpotLight") {
            auto casted = std::dynamic_pointer_cast<ISpotLightParams>(currentEntity->GetEntityParams());
            if (casted) {
                glm::vec3 lightDirection = casted->GetDirection();

                ImGui::Separator();
                ImGui::SeparatorText("Direction");
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("X", &lightDirection.x, 0.01f);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Y", &lightDirection.y, 0.01f);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("Z", &lightDirection.z, 0.01f);

                casted->SetDirection(lightDirection);

                ImGui::Separator();
                ImGui::SeparatorText("Calculation");

                float lightConstant = casted->GetConstant();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("constant", &lightConstant, 0.01f);

                casted->SetConstant(lightConstant);

                float lightLinear = casted->GetLinear();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("linear", &lightLinear, 0.01f);

                casted->SetLinear(lightLinear);

                float lightQuadratic = casted->GetQuadratic();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("quadratic", &lightQuadratic, 0.01f);

                casted->SetQuadratic(lightQuadratic);

                ImGui::Separator();
                ImGui::SeparatorText("Cones");

                float lightOuterCutOff = casted->GetOuterCutOff();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("outer", &lightOuterCutOff, 0.01f);

                casted->SetOuterCutOff(lightOuterCutOff);

                float lightCutOff = casted->GetCutOff();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("inner", &lightCutOff, 0.01f);

                casted->SetCutOff(lightCutOff);
            }
        }

        else if (currentEntity->GetEntityParams()->GetType() == "PointLight") {
            auto casted = std::dynamic_pointer_cast<IPointLightParams>(currentEntity->GetEntityParams());
            if (casted) {
                ImGui::Separator();
                ImGui::SeparatorText("Calculation");

                float lightConstant = casted->GetConstant();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("constant", &lightConstant, 0.01f);

                casted->SetConstant(lightConstant);

                float lightLinear = casted->GetLinear();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("linear", &lightLinear, 0.01f);

                casted->SetLinear(lightLinear);

                float lightQuadratic = casted->GetQuadratic();

                ImGui::SetNextItemWidth(itemWidth - padding);
                ImGui::DragFloat("quadratic", &lightQuadratic, 0.01f);

                casted->SetQuadratic(lightQuadratic);
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "TileMap") {
            auto casted = std::dynamic_pointer_cast<ITileMapParams>(currentEntity->GetEntityParams());
            if (casted) {
                std::shared_ptr<ITileMap> tileMap = std::dynamic_pointer_cast<ITileMap>(currentEntity);
                
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
            auto casted = std::dynamic_pointer_cast<IFlipBookParams>(currentEntity->GetEntityParams());
            if (casted) {
                std::shared_ptr<IFlipBook> flipBook = std::dynamic_pointer_cast<IFlipBook>(currentEntity);

                ImGui::Separator();
                ImGui::SeparatorText("FlipBook");

                if (ImGui::Button("Edit this FlipBook")) {
                    WindowFlipBookEdit::GetInstance().showWindow = true;
                    WindowFlipBookEdit::GetInstance().edittingFlipBook = flipBook;
                }

                if (!flipBook->GetFrames().empty()) {
                    float frameWait = casted->GetFrameWait();
                    bool loop = casted->GetLoop();

                    ImGui::DragFloat("Frame Duration", &frameWait, 0.001f, 0.0f);
                    ImGui::Checkbox("Loop", &loop);

                    //TODO: Add setter functions
                    casted->SetFrameWait(frameWait);
                    casted->SetLoop(loop);

                    if (ImGui::Button("Restart FlipBook"))
                        flipBook->RestartFlipBook();
                }
            }
        }
        else if (currentEntity->GetEntityParams()->GetType() == "Camera") {
            auto casted = std::dynamic_pointer_cast<ICameraParams>(currentEntity->GetEntityParams());
            if (casted) {
                std::shared_ptr<ICamera> flipBook = std::dynamic_pointer_cast<ICamera>(currentEntity);

                ImGui::Separator();
                ImGui::SeparatorText("Projection");

                ImGui::Text("Projection Type"); ImGui::SameLine();

                //Selection for the type of the scene
                CameraProjection projectionType = casted->GetProjectionType();

                ImGui::RadioButton("2D", &(int&)projectionType, (int)CameraProjection::ORTHOGRAPHIC); ImGui::SameLine();
                ImGui::RadioButton("3D", &(int&)projectionType, (int)CameraProjection::PERPECTIVE);

                casted->SetProjectionType(projectionType);

                if (projectionType == CameraProjection::ORTHOGRAPHIC) {
                    ImGui::Separator();
                    ImGui::SeparatorText("Orthographic");
                }
                else if(projectionType == CameraProjection::PERPECTIVE) {
                    ImGui::Separator();
                    ImGui::SeparatorText("Perspective");

                    float fov = casted->GetFOV();

                    ImGui::DragFloat("FOV", &fov, 1.0f, 10.0f, 170.0f);

                    casted->SetFOV(fov);
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
void WindowEntityProperties::SelectEntity(std::shared_ptr<IEntity> entity)
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