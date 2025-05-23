#pragma once

#include "EntityParams.h"
#include "AssetDatabase.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Sprite2DParams : public EntityParams {
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "Sprite2D";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<Sprite2DParams>(*this);
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

		//Save some properties
		j["r"] = r;
		j["g"] = g;
		j["b"] = b;
		j["a"] = a;

		//Save the id of the texture
		if (!textureId.empty())
			j["texture-id"] = textureId;

		//Save the type
		j["type"] = GetType();

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j) override;

	//Variables for the Sprite2D
	std::string textureId = "";
	unsigned int texture = 0;
	float r = 0, b = 0, g = 0, a = 0;
};