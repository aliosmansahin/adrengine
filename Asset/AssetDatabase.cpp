#include "pch.h"
#include "AssetDatabase.h"
#include "Graphics.h"

/*
PURPOSE: Creates a texture asset from a path and insert it to the textures map
*/
bool AssetDatabase::CreateTexture(std::string path, std::string name)
{
	//Set the texture id
	int index = 0;
	std::string textureId;
	while (true) {
		textureId = "texture" + std::to_string(index);
		if (textures.find(textureId) == textures.end())
			break;
		++index;
	}

	//Load the texture
	int texW, texH;
	unsigned int texture = Graphics::GetInstance().LoadTexture(textureId.c_str(), path.c_str(), texW, texH);
	if (texture == -1)
		return false;

	//Create a texture asset and insert it to the map
	std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>();
	texturePtr->id = textureId;
	if(name == "")
		texturePtr->name = textureId;
	else
		texturePtr->name = name;
	texturePtr->path = path;
	texturePtr->texture = texture;
	texturePtr->width = texW;
	texturePtr->height = texH;

	textures.insert({ textureId, texturePtr });

	return true;
}

/*
PURPOSE: Loads a texture asset from a path, and insert it to the map
*/
bool AssetDatabase::LoadTexture(std::string id, std::string path, std::string name)
{
	//Load the texture
	int texW, texH;
	unsigned int texture = Graphics::GetInstance().LoadTexture(id.c_str(), path.c_str(), texW, texH);
	if (texture == -1)
		return false;

	//Create a texture asset and insert it to the map
	std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>();
	texturePtr->id = id;
	texturePtr->name = name;
	texturePtr->path = path;
	texturePtr->texture = texture;
	texturePtr->width = texW;
	texturePtr->height = texH;

	textures.insert({ id, texturePtr });

	return true;
}

/*
PURPOSE: Load all assets from their paths given in the asset_database file
*/
bool AssetDatabase::LoadDatabase(std::string databasePath)
{
	/*
		Loading all textures from a database file with json,
		use id : path relationships,
		after that create each texture
	*/

	//Open the file and check if it exists
	std::ifstream file(databasePath);
	if (!file.is_open()) {
		Logger::Log("E", "Unable to open file for loading asset database.");
		return false;
	}

	nlohmann::json j;
	file >> j;

	//Load textures
	if (!j.contains("textures")) {
		std::string str = "Failed to load database file \"";
		str += databasePath;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}
	for (auto& t : j["textures"]) {
		LoadTexture(t["id"], t["path"], t["name"]);
	}

	//Load meshes
	if (!j.contains("meshes")) {
		std::string str = "Failed to load database file \"";
		str += databasePath;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}
	for (auto& t : j["meshes"]) {
		LoadMesh(t["id"], t["path"], t["name"]);
	}

	return true;
}

/*
PURPOSE: Saves all assets into a asset_database
*/
bool AssetDatabase::SaveDatabase(std::string databasePath)
{
	nlohmann::json j;
	//Create json from each texture
	for (auto& texture : textures) {
		nlohmann::json t;
		t["id"] = texture.second->id;
		t["name"] = texture.second->name;
		t["path"] = texture.second->path;
		j["textures"].push_back(t);
	}

	//Create json from each mesh
	for (auto& mesh : meshes) {
		nlohmann::json t;
		t["id"] = mesh.second->id;
		t["name"] = mesh.second->name;
		t["path"] = mesh.second->path;
		j["meshes"].push_back(t);
	}

	//Save the json into a file
	std::ofstream file(databasePath);
	if (!file.is_open()) {
		Logger::Log("E", "Unable to open file for saving asset database.");
		return false;
	}

	file << j.dump(4);

	return true;
}

/*
PURPOSE: Deletes a texture asset which is given id from all textures
*/
bool AssetDatabase::DeleteTexture(std::string id)
{
	//Check if the texture exists
	auto iter = textures.find(id);
	if (iter == textures.end()) {
		std::string str = "There is not any texture has id \"";
		str += id;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}

	auto& texture = iter->second;
	if (!texture.get())
		return false;

	//Delete texture from graphics and the database
	Graphics::GetInstance().UnloadTexture(texture->texture);
	textures.erase(iter);

	return true;
}

/*
PURPOSE: Returns a texture asset from an id which is given
*/
std::shared_ptr<Texture> AssetDatabase::GetTexture(std::string id)
{
	auto textureIter = textures.find(id);
	if (textureIter == textures.end())
		return nullptr;
	if (!textureIter->second)
		return nullptr;
	return textureIter->second;
}

/*
PURPOSE: Creates a mesh asset from a path and insert it to the meshes map
*/
ASSET_API bool AssetDatabase::CreateMesh(std::string path, std::string name)
{
	//Set the mesh id
	int index = 0;
	std::string meshId;
	while (true) {
		meshId = "mesh" + std::to_string(index);
		if (meshes.find(meshId) == meshes.end())
			break;
		++index;
	}

	//Load the mesh
	int verticeCount;
	unsigned int mesh = Graphics::GetInstance().LoadMesh(meshId.c_str(), path.c_str(), verticeCount);
	if (mesh == -1)
		return false;

	//Create a mesh asset and insert it to the map
	std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>();
	meshPtr->id = meshId;
	if (name == "")
		meshPtr->name = meshId;
	else
		meshPtr->name = name;
	meshPtr->path = path;
	meshPtr->meshVAO = mesh;
	meshPtr->verticeCount = verticeCount;

	meshes.insert({ meshId, meshPtr });
	return true;
}

/*
PURPOSE: Loads a mesh asset from a path, and insert it to the map
*/
ASSET_API bool AssetDatabase::LoadMesh(std::string id, std::string path, std::string name)
{
	//Load the mesh
	int verticeCount;
	unsigned int mesh = Graphics::GetInstance().LoadMesh(id.c_str(), path.c_str(), verticeCount);
	if (mesh == -1)
		return false;

	//Create a mesh asset and insert it to the map
	std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>();
	meshPtr->id = id;
	meshPtr->name = name;
	meshPtr->path = path;
	meshPtr->meshVAO = mesh;
	meshPtr->verticeCount = verticeCount;

	meshes.insert({ id, meshPtr });
	return true;
}

/*
PURPOSE: Deletes a mesh asset which is given id from all meshes
*/
ASSET_API bool AssetDatabase::DeleteMesh(std::string id)
{
	//Check if the mesh exists
	auto iter = meshes.find(id);
	if (iter == meshes.end()) {
		std::string str = "There is not any mesh has id \"";
		str += id;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}

	auto& mesh = iter->second;
	if (!mesh.get())
		return false;

	//Delete mesh from the database
	Graphics::GetInstance().UnloadMesh(mesh->meshVAO, mesh->meshVBOpositions, mesh->meshVBOnormals, mesh->meshVBOuvs);
	meshes.erase(iter);

	return true;
}

/*
PURPOSE: Returns a mesh asset from an id which is given
*/
ASSET_API std::shared_ptr<Mesh> AssetDatabase::GetMesh(std::string id)
{
	auto meshIter = meshes.find(id);
	if (meshIter == meshes.end())
		return nullptr;
	if (!meshIter->second)
		return nullptr;
	return meshIter->second;
}

/*
PURPOSE: Gets the instance of the class
*/
AssetDatabase& AssetDatabase::GetInstance()
{
	static AssetDatabase db;
	return db;
}
