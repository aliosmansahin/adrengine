#pragma once

#include <glm/glm.hpp>
#include "../../../../../nlohmann_json/json.hpp"

class IFlipBookFrame
{
public:
	virtual ~IFlipBookFrame() = default;

	//main
	virtual void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH) = 0;
	virtual void Update() = 0;
	virtual void Draw(int tileW, int tileH, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) = 0;
	virtual void Release() = 0;

	//json
	virtual nlohmann::json ToJson() = 0;

	//getters
	virtual std::pair<int, int> GetXY() = 0;
	virtual std::pair<int, int> GetSize() = 0;
	virtual std::pair<float, float> GetUV() = 0;
	virtual std::pair<float, float> GetTextureSize() = 0;

};

