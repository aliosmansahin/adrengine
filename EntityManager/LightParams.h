#pragma once

#include "EntityParams.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "interfaces/IEntity/ILight/ILightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class LightParams : public virtual EntityParams, public virtual ILightParams
{
public:
	ENTITYMANAGER_API virtual ~LightParams() = default;

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
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	/*
	PURPOSE: Getter for the color
	*/
	ENTITYMANAGER_API glm::vec3 GetColor() override {
		return color;
	}
	ENTITYMANAGER_API void SetColor(glm::vec3 color) override {
		this->color = color;
	}

protected:
	//common light matrices
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

