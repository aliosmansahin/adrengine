#pragma once

#include "glad/glad.h"
#include "ShaderManager.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Tile
{
public:
	//main funcs
	void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH);
	void Update();
	void Draw(int tileW, int tileH, unsigned int tileMap, unsigned int tileVAO);
	void Release();
public:
	int x, y;
	int width, height;
	float u, v;
	float textureWidth, textureHeight;

	//drawing tile
	unsigned int tileVAO = -1;
	unsigned int tileVBO = -1;
	unsigned int tileEBO = -1;
};
