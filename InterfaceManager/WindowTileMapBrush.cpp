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
        if (ImGui::Button("Start drawing")) {
            editing = true;
        }

        ImGui::SeparatorText("Tiles");
        ImVec2 windowPos = ImGui::GetCursorPos();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        int padding = 10;

        for (auto& tile : tiles) {
            pos = ImGui::GetCursorScreenPos();

            //Pass the coordinates
            int tileX = tile.second->x;
            int tileY = tile.second->y;
            int currentTileWidth = tile.second->width;
            int currentTileHeight = tile.second->height;
            float u = tile.second->u;
            float v = tile.second->v;
            float tw = tile.second->textureWidth;
            float th = tile.second->textureHeight;

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

            pos.x += currentTileWidth;
            pos.y += currentTileHeight;
        }

        ImVec2 deltaPos;
        deltaPos.x = pos.x - startPos.x;
        deltaPos.y = pos.y - startPos.y;

        ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + deltaPos.y + padding));
    }

    ImGui::End();
}

WindowTileMapBrush& WindowTileMapBrush::GetInstance()
{
	static WindowTileMapBrush window;
	return window;
}
