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

class LightParams : public EntityParams
{
public:

	/*
	PURPOSE: Creates json content from properties of the entity
	*/
	ENTITYMANAGER_API nlohmann::json ToJson() override {
		auto j = EntityParams::ToJson();

		j["color-x"] = color.x;
		j["color-y"] = color.y;
		j["color-z"] = color.z;
		
		return j;
	}

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir) override;

	//common light matrices
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

