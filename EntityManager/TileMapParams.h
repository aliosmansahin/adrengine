#pragma once

#include "Sprite2DParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class TileMapParams : public Sprite2DParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "TileMap";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<TileMapParams>(*this);
	}

	/*
	PURPOSE: Creates json content from properties of the entity
	*/
	ENTITYMANAGER_API nlohmann::json ToJson() override {
		/*
			Its kind of "super" function.
			Which calls the function of the base class.
			In this function, we are overriding the function, so we need it
		*/
		auto j = EntityParams::ToJson();

		//Save the id of the texture
		if (!textureId.empty())
			j["texture-id"] = textureId;

		//Save the type
		j["type"] = GetType();

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, IScene* scene) override;

	//tilemap texture
	std::string textureId = "";
	unsigned int texture = 0;
};

