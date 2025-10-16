#pragma once

#include "glad/glad.h"
#include "ShaderManager.h"

#include "interfaces/IEntity/ISprite2D/ITileMap/ITile/ITile.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Tile : public ITile
{
public:
	virtual ~Tile() = default;

	//main funcs
	void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH, std::pair<int, int> tileType) override;
	void Update() override;
	void Draw(int tileW, int tileH, int translateX, int translateY, int translateZ = 0.0f) override;
	void Release() override;

	//Getters and setters
	void SetPos(int x, int y) override;

	//json
	nlohmann::json ToJson() override;

	//getters
	ENTITYMANAGER_API std::pair<int, int> GetXY() override;
	ENTITYMANAGER_API std::pair<int, int> GetSize() override;
	ENTITYMANAGER_API std::pair<float, float> GetUV() override;
	ENTITYMANAGER_API std::pair<float, float> GetTextureSize() override;

private:
	//position of the tile on the preview or on the scene
	/*
		x=1 means 1 * tileWidth value of the map
		width and height is for storing the texture size
	*/
	int x, y;
	int width, height;

	//coordinates to store which part of texture will be shown
	float u, v;
	float textureWidth, textureHeight;

	//drawing tile
	unsigned int tileVAO = -1;
	unsigned int tileVBO = -1;
	unsigned int tileEBO = -1;

	std::pair<int, int> tileType = { -1, -1 };
};
