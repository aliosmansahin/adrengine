#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "nlohmann_json/json.hpp"

#include "Graphics.h"

struct Texture {
	std::string id;
	std::string name;
	unsigned int texture;
	std::string path;
	int width, height;
};

class AssetDatabase
{
public:
	bool CreateTexture(std::string path, std::string name = "");
	bool LoadTexture(std::string id, std::string path, std::string name);
	bool LoadDatabase(std::string databasePath);
	bool SaveDatabase(std::string databasePath);
	bool DeleteTexture(std::string id);
	std::shared_ptr<Texture> GetTexture(std::string id);
	std::unordered_map<std::string, std::shared_ptr<Texture>>& GetTextures() {
		return textures;
	}
	static AssetDatabase& GetInstance();
private:
	AssetDatabase() = default;
	~AssetDatabase() = default;
	AssetDatabase(const AssetDatabase&) = delete;
	AssetDatabase& operator=(const AssetDatabase&) = delete;
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};