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

#include "Graphics.h"

#include "nlohmann_json/json.hpp"

//Texture Asset
struct Texture {
	std::string id; //Unique id
	std::string name; //Name to display
	std::string path; //Texture path
	unsigned int texture; //GL texture
	int width, height; //Texture size
};

//Mesh asset
struct Mesh {
	std::string id; //Unique id
	std::string name; //Name to display
	std::string path; //Texture path
	std::vector<std::shared_ptr<ObjectMtl>> objects; //Objects with materials, all of them makes the mesh
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
	//helpers
	template <typename mapT>
	std::string CreateID(std::string prefix, std::map<std::string, std::shared_ptr<mapT>>& mapForIdCheck);

	template <typename mapT>
	bool CheckAssetExists(
		std::string id,
		std::map<std::string, std::shared_ptr<mapT>>& mapForCheck,
		typename std::map<std::string, std::shared_ptr<mapT>>::iterator& outIter);

	template <typename mapT>
	std::shared_ptr<mapT> GetAsset(std::string id, std::map<std::string, std::shared_ptr<mapT>>& mapToSearch);
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