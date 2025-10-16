#pragma once

#include "../ISprite2D.h"
#include "ITileMapParams.h"

#include "ITile/ITile.h"

class ITileMap : public virtual ISprite2D
{
public:
	virtual ~ITileMap() = default;

	virtual void CreateTiles(float textureWidth, float textureHeight, float tileWidth, float tileHeight) = 0;
	virtual void CreateInspectFrameBuffer(float width, float height, int tileW, int tileH, float scale) = 0;
	virtual void UpdateInspect() = 0;
	virtual void DrawInspect(int width, int height, int tileW, int tileH, float scale) = 0;
	virtual bool IsInspectCreated() = 0;
	virtual unsigned int GetInspectTexture() = 0;
	virtual std::map<std::pair<int, int>, std::shared_ptr<ITile>>& GetCreatedTiles() = 0;

	virtual void AddTileToMap(int mouseX, int mouseY, float cameraX, float cameraY, std::pair<int, int> selectedTile) = 0;
	virtual void RemoveTileFromMap(int mouseX, int mouseY, float cameraX, float cameraY) = 0;
	virtual void UpdateMouseTileIndicator(int mouseX, int mouseY, float cameraX, float cameraY) = 0;
	virtual std::pair<int, int> GetTilePos(float mouseX, float mouseY, float cameraX, float cameraY) = 0;

	virtual void FromJson(nlohmann::json json) = 0;

	//Getters
	virtual std::pair<int, int> GetTileSize() = 0;

	//Setters
	virtual void SetDrawingViewer(bool drawingViewer) = 0;
};

