#pragma once

#include "EntityParams.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class DirectionalLightParams : public EntityParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "DirectionalLight";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<DirectionalLightParams>(*this);
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
		j["type"] = GetType();
		j["direction-x"] = direction.x;
		j["direction-y"] = direction.y;
		j["direction-z"] = direction.z;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j) override;

	//Variables for the DirectionalLight
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f);
};