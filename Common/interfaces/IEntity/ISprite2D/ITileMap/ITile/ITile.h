#pragma once

#include "../../../../../nlohmann_json/json.hpp"

class ITile
{
public:
	virtual ~ITile() = default;

	//main funcs
	virtual void Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH, std::pair<int, int> tileType) = 0;
	virtual void Update() = 0;
	virtual void Draw(int tileW, int tileH, int translateX, int translateY, int translateZ = 0.0f) = 0;
	virtual void Release() = 0;

	//Getters and setters
	virtual void SetPos(int x, int y) = 0;

	//json
	virtual nlohmann::json ToJson() = 0;

	//getters
	virtual std::pair<int, int> GetXY() = 0;
	virtual std::pair<int, int> GetSize() = 0;
	virtual std::pair<float, float> GetUV() = 0;
	virtual std::pair<float, float> GetTextureSize() = 0;
};