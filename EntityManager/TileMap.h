#pragma once

#include "Sprite2D.h"
#include "TileMapParams.h"

#include "ShaderManager.h"

#include "Tile.h"

#include "utils/Utils.h"

#include "interfaces/IEntity/ISprite2D/ITileMap/ITileMap.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class TileMap : public virtual Sprite2D, public virtual ITileMap
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	//tiles
	ENTITYMANAGER_API void CreateTiles(float textureWidth, float textureHeight, float tileWidth, float tileHeight) override;
	ENTITYMANAGER_API void CreateInspectFrameBuffer(float width, float height, int tileW, int tileH, float scale) override;
	ENTITYMANAGER_API void UpdateInspect() override;
	ENTITYMANAGER_API void DrawInspect(int width, int height, int tileW, int tileH, float scale) override;
	ENTITYMANAGER_API bool IsInspectCreated() override;
	ENTITYMANAGER_API unsigned int GetInspectTexture() override;
	ENTITYMANAGER_API std::map<std::pair<int, int>, std::shared_ptr<ITile>>& GetCreatedTiles() override;

	ENTITYMANAGER_API void AddTileToMap(int mouseX, int mouseY, float cameraX, float cameraY, std::pair<int, int> selectedTile) override;
	ENTITYMANAGER_API void RemoveTileFromMap(int mouseX, int mouseY, float cameraX, float cameraY) override;
	ENTITYMANAGER_API void UpdateMouseTileIndicator(int mouseX, int mouseY, float cameraX, float cameraY) override;
	ENTITYMANAGER_API std::pair<int, int> GetTilePos(float mouseX, float mouseY, float cameraX, float cameraY) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<TileMap>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
	ENTITYMANAGER_API void FromJson(nlohmann::json json) override;

	//Getters
	ENTITYMANAGER_API std::pair<int, int> GetTileSize() override { return { tileWidth, tileHeight }; };

	//Setters

	ENTITYMANAGER_API void SetDrawingViewer(bool drawingViewer) override { this->drawingViewer = drawingViewer; };

private:
	//Is this tilemap drawing on tilemapviewer
	bool drawingViewer = false;

private:
	//properties
	std::shared_ptr<ITileMapParams> params;

	//tile size
	int tileWidth = 0;
	int tileHeight = 0;

	//inspector framebuffer
	FramebufferProvider* inspectFramebuffer = nullptr;

	//Store created tiles from tilemap
	std::map<std::pair<int, int>, std::shared_ptr<ITile>> createdTiles;

	//Store tiles to draw them on the scene
	std::map<std::pair<int, int>, std::shared_ptr<ITile>> tiles;

	//Store tile indicator
	std::shared_ptr<ITile> tileIndicator;
	bool drawTileIndicator = false;
};