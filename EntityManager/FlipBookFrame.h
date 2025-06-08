#pragma once

#include "glad_wrapper.h"
#include "ShaderManager.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

using namespace adr;

class FlipBookFrame {
public:
	//main funcs
	ENTITYMANAGER_API void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH);
	ENTITYMANAGER_API void Update();
	ENTITYMANAGER_API void Draw(int tileW, int tileH, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	ENTITYMANAGER_API void Release();

	//json
	ENTITYMANAGER_API nlohmann::json ToJson();
public:
	int x, y;
	int width, height; //texture size

	//coordinates to store which part of texture will be shown
	float u, v;
	float textureWidth, textureHeight;
private:
	//drawing frame
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;
};