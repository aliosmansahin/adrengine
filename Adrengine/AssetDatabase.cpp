#include "AssetDatabase.h"

bool AssetDatabase::CreateTexture(std::string path, std::string name)
{
	int index = 0;
	std::string textureId;
	while (true) {
		textureId = "texture" + std::to_string(index);
		if (textures.find(textureId) == textures.end())
			break;
		++index;
	}

	int texW, texH;

	unsigned int texture = Graphics::GetInstance().LoadTexture(textureId.c_str(), path.c_str(), texW, texH);
	if (texture == -1)
		return false;

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

bool AssetDatabase::LoadTexture(std::string id, std::string path, std::string name)
{
	int texW, texH;

	unsigned int texture = Graphics::GetInstance().LoadTexture(id.c_str(), path.c_str(), texW, texH);
	if (texture == -1)
		return false;

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

bool AssetDatabase::LoadDatabase(std::string databasePath)
{
	//loading all textures from a database file with json, use id: path relationships, after that create each texture
	std::ifstream file(databasePath);
	if (!file.is_open()) {
		Logger::Log("E", "Unable to open file for loading asset database.");
		return false;
	}

	nlohmann::json j;
	file >> j;

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

	return true;
}

bool AssetDatabase::SaveDatabase(std::string databasePath)
{
	nlohmann::json j;
	for (auto& texture : textures) {
		nlohmann::json t;
		t["id"] = texture.second->id;
		t["name"] = texture.second->name;
		t["path"] = texture.second->path;
		j["textures"].push_back(t);
	}

	std::ofstream file(databasePath);
	if (!file.is_open()) {
		Logger::Log("E", "Unable to open file for saving asset database.");
		return false;
	}

	file << j.dump(4);

	return true;
}

bool AssetDatabase::DeleteTexture(std::string id)
{
	auto iter = textures.find(id);
	if (iter == textures.end()) {
		std::string str = "There is not any texture has id \"";
		str += id;
		str += " \"";

		Logger::Log("E", str.c_str());
		return false;
	}

	//delete texture from the database
	auto& texture = iter->second;
	if (!texture.get())
		return false;

	textures.erase(iter);

	return true;
}

std::shared_ptr<Texture> AssetDatabase::GetTexture(std::string id)
{
	auto textureIter = textures.find(id);
	if (textureIter == textures.end())
		return nullptr;
	if (!textureIter->second)
		return nullptr;
	return textureIter->second;
}

AssetDatabase& AssetDatabase::GetInstance()
{
	static AssetDatabase db;
	return db;
}
