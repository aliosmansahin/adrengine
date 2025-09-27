#include "pch.h"
#include "AssetDatabase.h"

/*
PURPOSE: Creates a texture asset from a path and insert it to the textures map
*/
bool AssetDatabase::CreateTexture(std::string path, std::string name)
{
	//Set the texture id
	std::string textureId = CreateID("texture", textures);

	//Load and add a new texture object
	if (!LoadTexture(textureId, path, name))
		return false;

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
	if (name == "")
		texturePtr->name = id;
	else
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
		std::string str = "No textures detected in database file \"";
		str += databasePath;
		str += "\"";

		Logger::Log("I", str.c_str());
	}
	else {
		for (auto& t : j["textures"]) {
			LoadTexture(t["id"], t["path"], t["name"]);
		}
	}

	//Load meshes
	if (!j.contains("meshes")) {
		std::string str = "No meshes detected in database file \"";
		str += databasePath;
		str += "\"";

		Logger::Log("I", str.c_str());
	}
	else {
		for (auto& t : j["meshes"]) {
			LoadMesh(t["id"], t["path"], t["name"]);
		}
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
	auto textureIter = textures.end();
	if (!CheckAssetExists(id, textures, textureIter))
		return false;

	//Check for the outIter
	if (textureIter == textures.end())
		return false;

	//Get texture pointer
	auto& texture = textureIter->second;

	//Check nullptr
	if (!texture.get())
		return false;

	//Delete texture from graphics and the database
	Graphics::GetInstance().UnloadTexture(texture->texture);
	textures.erase(textureIter);

	return true;
}

/*
PURPOSE: Returns a texture asset from an id which is given
*/
std::shared_ptr<Texture> AssetDatabase::GetTexture(std::string id)
{
	return GetAsset(id, textures);
}

/*
PURPOSE: Creates a mesh asset from a path and insert it to the meshes map
*/
ASSET_API bool AssetDatabase::CreateMesh(std::string path, std::string name)
{
	//Set the mesh id
	std::string meshId = CreateID("mesh", meshes);

	//Load and add a new mesh object
	if (!LoadMesh(meshId, path, name))
		return false;

	return true;
}

/*
PURPOSE: Loads a mesh asset from a path, and insert it to the map
*/
ASSET_API bool AssetDatabase::LoadMesh(std::string id, std::string path, std::string name)
{
	//Create a mesh asset and insert it to the map
	std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>();
	meshPtr->id = id;
	if (name == "")
		meshPtr->name = id;
	else
		meshPtr->name = name;
	meshPtr->path = path;

	//Load the mesh
	bool result = Graphics::GetInstance().LoadMesh(id.c_str(), path.c_str(), meshPtr->objects);
	if (!result || meshPtr->objects.empty())
		return false;

	meshes.insert({ id, meshPtr });

	return true;
}

/*
PURPOSE: Deletes a mesh asset which is given id from all meshes
*/
ASSET_API bool AssetDatabase::DeleteMesh(std::string id)
{
	//Check if the mesh exists
	auto meshIter = meshes.end();
	if (!CheckAssetExists(id, meshes, meshIter))
		return false;

	//Check for the outIter
	if (meshIter == meshes.end())
		return false;

	//Get mesh pointer
	auto& mesh = meshIter->second;
	
	//Check nullptr
	if (!mesh.get())
		return false;

	//Delete mesh from the database
	Graphics::GetInstance().UnloadMesh(mesh->objects);
	meshes.erase(meshIter);

	return true;
}

/*
PURPOSE: Returns a mesh asset from an id which is given
*/
ASSET_API std::shared_ptr<Mesh> AssetDatabase::GetMesh(std::string id)
{
	return GetAsset(id, meshes);
}

/*
PURPOSE: Gets the instance of the class
*/
AssetDatabase& AssetDatabase::GetInstance()
{
	static AssetDatabase db;
	return db;
}

/*
PURPOSE: Creates an id with a prefix
	Ensures created id doesn't exist in the given map
*/
template <typename mapT>
std::string AssetDatabase::CreateID(std::string prefix, std::map<std::string, std::shared_ptr<mapT>>& mapForIdCheck)
{
	int index = 0;
	std::string id;

	while (true) {
		id = prefix + std::to_string(index);
		if (mapForIdCheck.find(id) == mapForIdCheck.end())
			break;
		++index;
	}

	return id;
}

/*
PURPOSE: Checks if there is an asset which has given id exists
	outIter will be set to checking asset
*/
template<typename mapT>
bool AssetDatabase::CheckAssetExists(
	std::string id,
	std::map<std::string, std::shared_ptr<mapT>>& mapForCheck,
	typename std::map<std::string, std::shared_ptr<mapT>>::iterator& outIter)
{
	outIter = mapForCheck.find(id);
	if (outIter == mapForCheck.end()) {
		std::string str = "There is not any asset has id \"";
		str += id;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}

	return true;
}

/*
PURPOSE: Gets asset from given map with id
*/
template<typename mapT>
std::shared_ptr<mapT> AssetDatabase::GetAsset(std::string id, std::map<std::string, std::shared_ptr<mapT>>& mapToSearch)
{
	auto iter = mapToSearch.find(id);
	if (iter == mapToSearch.end())
		return nullptr;
	if (!iter->second)
		return nullptr;
	return iter->second;
}
