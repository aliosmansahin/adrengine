#pragma once

#include "Sprite2D.h"
#include "TileMapParams.h"

#include "ShaderManager.h"

#include "Tile.h"

#include "utils/Utils.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class TileMap : public Sprite2D
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<EntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	//tiles
	ENTITYMANAGER_API void CreateTiles(float textureWidth, float textureHeight, float tileWidth, float tileHeight);
	ENTITYMANAGER_API void CreateInspectFrameBuffer(float width, float height, int tileW, int tileH, float scale);
	ENTITYMANAGER_API void UpdateInspect();
	ENTITYMANAGER_API void DrawInspect(int width, int height, int tileW, int tileH, float scale);
	ENTITYMANAGER_API bool IsInspectCreated();
	ENTITYMANAGER_API unsigned int GetInspectTexture();
	ENTITYMANAGER_API std::map<std::pair<int, int>, std::shared_ptr<Tile>>& GetCreatedTiles();

	ENTITYMANAGER_API static void AddTileToMap(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY, std::pair<int, int> selectedTile);
	ENTITYMANAGER_API static void RemoveTileFromMap(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY);
	ENTITYMANAGER_API static void UpdateMouseTileIndicator(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY);

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<Entity> clone() const override {
		return std::make_shared<TileMap>(*this);
	}

	//properties
	ENTITYMANAGER_API EntityParams* GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
	ENTITYMANAGER_API void FromJson(nlohmann::json json);
public:
	//tile size
	int tileWidth = 0;
	int tileHeight = 0;
private:
	//properties
	std::shared_ptr<TileMapParams> params;

	//inspector framebuffer
	unsigned int inspectFrameBuffer = -1;
	unsigned int inspectRenderBuffer = -1;
	unsigned int inspectTexture = -1;

	//drawing tilemap
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;

	//Store created tiles from tilemap
	std::map<std::pair<int, int>, std::shared_ptr<Tile>> createdTiles;

	//Store tiles to draw them on the scene
	std::map<std::pair<int, int>, std::shared_ptr<Tile>> tiles;

	//Store tile indicator
	std::shared_ptr<Tile> tileIndicator;
	bool drawTileIndicator = false;
};