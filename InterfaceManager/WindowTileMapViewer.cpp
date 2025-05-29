#include "pch.h"
#include "WindowTileMapViewer.h"

/*
PURPOSE: Draws the window
*/
void WindowTileMapEdit::DrawWindow()
{
    if (!editingTileMap)
        return;
    TileMapParams* params = dynamic_cast<TileMapParams*>(editingTileMap->GetEntityParams());
    if (!params)
        return;

	ImGui::Begin("Edit Tile Map", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    //Store entity id
    std::string textureIdStr = params->textureId;

    static char textureIdBuf[32] = "";

    if (textureIdBuf != textureIdStr) { // if the entity id changed
        strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
    }

    //Get texture Id
    if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) { //CHANGE IT TO OPTION PANEL THAT CONTAINS ALL TEXTURES
        if (textureIdBuf[0] == '\0') {// if the input is empty
            strncpy_s(textureIdBuf, params->name.c_str(), sizeof(textureIdBuf));
            textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
        }
        else {
            if (AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf)).get()) {
                params->textureId = std::string(textureIdBuf);
                params->texture = AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf))->texture;
            }
        }
    }

    //Drag drop for texture id
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ID")) {
            const char* payloadData = static_cast<const char*>(payload->Data);
            std::string droppedId(payloadData, payload->DataSize);

            auto& textures = AssetDatabase::GetInstance().GetTextures();
            auto textureIter = textures.find(droppedId);

            if (textureIter != textures.end()) {
                auto& texture = textureIter->second;
                if (texture.get()) {
                    params->textureId = texture->id;
                    params->texture = texture->texture;
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    //Draw the inspector texture for tile map
    if (!params->textureId.empty()) {
        //Get texture size
        auto& textures = AssetDatabase::GetInstance().GetTextures();
        auto textureIter = textures.find(params->textureId);
        int textureWidth = textureIter->second->width;
        int textureHeight = textureIter->second->height;

        //Tile size
        static int tileWidth = 32;
        static int tileHeight = 32;

        //Scaling doesn't work correctly
        float inspectScale = 1.0f;

        //Create framebuffer for inspector
        if (ImGui::Button("Create Inspector")) {
            editingTileMap->CreateInspectFrameBuffer((float)textureWidth, (float)textureHeight, tileWidth, tileHeight, inspectScale);
        }

        if (textureIter != textures.end() && editingTileMap->IsInspectCreated()) {
            //Update and render the inspector to the framebuffer
            editingTileMap->UpdateInspect();
            editingTileMap->DrawInspect(textureWidth, textureHeight, tileWidth, tileHeight, inspectScale);

            //Draw the inspector
            ImGui::SeparatorText("Inspector");
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 windowPos = ImGui::GetCursorPos();
            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(intptr_t)editingTileMap->GetInspectTexture(),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + textureWidth * inspectScale, pos.y + textureHeight * inspectScale),
                ImVec2(0, 0),
                ImVec2(1, 1)
            );

            //Some visuals
            int padding = 10;
            ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + textureHeight * inspectScale + padding));

            //Write parameters
            ImGui::Text(std::string("Texture Size: " + std::to_string(textureWidth) + "x" + std::to_string(textureHeight) + "px").c_str());
            ImGui::Text(std::string("Tile Size: " + std::to_string(tileWidth) + "x" + std::to_string(tileHeight) + "px").c_str());

            //Set the size of each tiles
            ImGui::DragInt("Tile Width", &tileWidth);
            ImGui::DragInt("Tile Height", &tileHeight);

            if (ImGui::Button("Create Tiles")) {
                //Create each tile
                editingTileMap->CreateTiles((float)textureWidth, (float)textureHeight, (float)tileWidth, (float)tileHeight);
            }

            //Draw preview of tiles
            auto& tiles = editingTileMap->GetCreatedTiles();
            if (!tiles.empty()) {
                ImGui::SeparatorText("Created Tiles");
                ImVec2 windowPos = ImGui::GetCursorPos();
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 startPos = ImGui::GetCursorScreenPos();

                for (auto& tile : tiles) {
                    pos = ImGui::GetCursorScreenPos();
                    
                    //Pass the coordinates
                    int tileX             = tile.second->x;
                    int tileY             = tile.second->y;
                    int currentTileWidth  = tile.second->width;
                    int currentTileHeight = tile.second->height;
                    float u               = tile.second->u;
                    float v               = tile.second->v;
                    float tw              = tile.second->textureWidth;
                    float th              = tile.second->textureHeight;

                    //Set the position of the preview of the tile
                    pos.x += tileX * (tileWidth + padding);
                    pos.y += tileY * (tileHeight + padding);

                    //Draw the tile
                    ImGui::GetWindowDrawList()->AddImage(
                        (ImTextureID)(intptr_t)params->texture,
                        ImVec2(pos.x,                            pos.y),
                        ImVec2(pos.x + currentTileWidth, pos.y + currentTileHeight),
                        ImVec2(u,      v),
                        ImVec2(u + tw, v + th)
                    );

                    pos.x += currentTileWidth;
                    pos.y += currentTileHeight;
                }

                ImVec2 deltaPos;
                deltaPos.x = pos.x - startPos.x;
                deltaPos.y = pos.y - startPos.y;

                ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + deltaPos.y + padding));

                if (ImGui::Button("Done")) {
                    showWindow = false;
                    editingTileMap = nullptr;
                }
            }
        }
    }

	ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
WindowTileMapEdit& WindowTileMapEdit::GetInstance()
{
	static WindowTileMapEdit window;
	return window;
}
