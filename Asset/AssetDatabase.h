#pragma once

#ifdef ASSET_EXPORTS
#define ASSET_API __declspec(dllexport)
#else
#define ASSET_API __declspec(dllimport)
#endif

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>

#include "nlohmann_json/json.hpp"

//Texture asset
struct Texture {
	std::string id;
	std::string name;
	std::string path;
	unsigned int texture;
	int width, height;
};

//Mesh asset
struct Mesh {
	std::string id;
	std::string name;
	std::string path;
	unsigned int meshVAO;
	unsigned int meshVBOpositions;
	unsigned int meshVBOnormals;
	unsigned int meshVBOuvs;
	int verticeCount;
};

class AssetDatabase
{
public:
	//texture functions
	ASSET_API bool											   CreateTexture(std::string path, std::string name = "");
	ASSET_API bool											   LoadTexture(std::string id, std::string path, std::string name);
	ASSET_API bool											   DeleteTexture(std::string id);
	ASSET_API std::shared_ptr<Texture>						   GetTexture(std::string id);
	ASSET_API std::map<std::string, std::shared_ptr<Texture>>& GetTextures() {
		return textures;
	}

	//mesh functions
	ASSET_API bool											   CreateMesh(std::string path, std::string name = "");
	ASSET_API bool											   LoadMesh(std::string id, std::string path, std::string name);
	ASSET_API bool											   DeleteMesh(std::string id);
	ASSET_API std::shared_ptr<Mesh>							   GetMesh(std::string id);
	ASSET_API std::map<std::string, std::shared_ptr<Mesh>>&	   GetMeshes() {
		return meshes;
	}

	//database functions
	ASSET_API bool											   LoadDatabase(std::string databasePath);
	ASSET_API bool											   SaveDatabase(std::string databasePath);

	//instance getter
	ASSET_API static AssetDatabase&							   GetInstance();
private:
	//singleton
	AssetDatabase() = default;
	~AssetDatabase() = default;
	AssetDatabase(const AssetDatabase&) = delete;
	AssetDatabase& operator=(const AssetDatabase&) = delete;
private:
	//database variables
	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::map<std::string, std::shared_ptr<Mesh>> meshes;
};