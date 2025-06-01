#include "pch.h"
#include "WindowTileMapBrush.h"

void WindowTileMapBrush::DrawWindow()
{
    if (!editingTileMap)
        return;
    TileMapParams* params = dynamic_cast<TileMapParams*>(editingTileMap->GetEntityParams());
    if (!params)
        return;

    ImGui::Begin("Tile Map Brush", &showWindow);
    ImGui::SetWindowFontScale(1.5f);

    //Draw preview of tiles
    auto& tiles = editingTileMap->GetCreatedTiles();
    if (!tiles.empty()) {
        //Button to switch drawing mode
        std::string editingButtonText = editing ? "Stop drawing" : "Start drawing";
        if (ImGui::Button(editingButtonText.c_str())) {
            editing = !editing;
            WindowTileMapViewer::GetInstance().showWindow = editing;
            if (editing)
                WindowTileMapViewer::GetInstance().edittingTileMap = editingTileMap;
            else 
                WindowTileMapViewer::GetInstance().edittingTileMap = nullptr;
        }

        ImGui::SeparatorText("Tiles");

        //Positions
        ImVec2 windowPos = ImGui::GetCursorPos();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        int padding = 10;
        int index = 0;

        for (auto& tile : tiles) {
            //Pass the coordinates
            int tileX = tile.second->x;
            int tileY = tile.second->y;
            int currentTileWidth = tile.second->width;
            int currentTileHeight = tile.second->height;
            float u = tile.second->u;
            float v = tile.second->v;
            float tw = tile.second->textureWidth;
            float th = tile.second->textureHeight;

            //Image Button spec
            ImVec2 imageSize = ImVec2((float)editingTileMap->tileWidth, (float)editingTileMap->tileHeight);
            std::string imageButtonId = "##image_button" + std::to_string(index);

            if (!editing) {
                pos = ImGui::GetCursorScreenPos();

                //Set the position of the preview of the tile
                pos.x += tileX * (editingTileMap->tileWidth + padding);
                pos.y += tileY * (editingTileMap->tileHeight + padding);

                //Draw the tile
                ImGui::GetWindowDrawList()->AddImage(
                    (ImTextureID)(intptr_t)params->texture,
                    ImVec2(pos.x, pos.y),
                    ImVec2(pos.x + currentTileWidth, pos.y + currentTileHeight),
                    ImVec2(u, v),
                    ImVec2(u + tw, v + th)
                );

                //After drawing, set the pos to end of the tiles
                pos.x += currentTileWidth;
                pos.y += currentTileHeight;
            }
            else {
                //Calculate current button positions
                ImVec2 currentButtonPos;
                currentButtonPos.x = windowPos.x + tileX * (editingTileMap->tileWidth + padding);
                currentButtonPos.y = windowPos.y + tileY * (editingTileMap->tileHeight + padding);

                //Change color when its selected
                bool selected = (tileX == selectedTile.first && tileY == selectedTile.second);
                ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.5, 1.0f);
                if(selected)
                    bgColor = ImVec4(0.0f, 1.0f, 0.0, 1.0f);

                //Set the position of the button
                ImGui::SetCursorPos(currentButtonPos);

                //Some color sets
                ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

                //Draw the tile
                if (ImGui::ImageButton(
                    imageButtonId.c_str(),
                    (ImTextureID)(intptr_t)params->texture,
                    imageSize,
                    ImVec2(u, v),
                    ImVec2(u + tw, v + th)
                )) {
                    selectedTile = { tileX, tileY };
                }

                //Disalbe color sets
                ImGui::PopStyleColor(2);
            }
            
            index++;
        }

        if (!editing) {
            //After drawing, set the cursor to bottom of the tiles
            ImVec2 deltaPos;
            deltaPos.x = pos.x - startPos.x;
            deltaPos.y = pos.y - startPos.y;

            ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + deltaPos.y + padding));
        }
    }

    ImGui::End();
}

WindowTileMapBrush& WindowTileMapBrush::GetInstance()
{
	static WindowTileMapBrush window;
	return window;
}
