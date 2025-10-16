#pragma once

#include "glad_wrapper.h"
#include "ShaderManager.h"

#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBookFrame/IFlipBookFrame.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

using namespace adr;

class FlipBookFrame : public IFlipBookFrame {
public:
	ENTITYMANAGER_API virtual ~FlipBookFrame() = default;

	//main funcs
	ENTITYMANAGER_API void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH) override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(int tileW, int tileH, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) override;
	ENTITYMANAGER_API void Release() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

	//getters
	ENTITYMANAGER_API std::pair<int, int> GetXY() override;
	ENTITYMANAGER_API std::pair<int, int> GetSize() override;
	ENTITYMANAGER_API std::pair<float, float> GetUV() override;
	ENTITYMANAGER_API std::pair<float, float> GetTextureSize() override;

private:
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