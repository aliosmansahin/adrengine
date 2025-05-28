#pragma once

#include "LightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class PointLightParams : public LightParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "PointLight";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<PointLightParams>(*this);
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
		auto j = LightParams::ToJson();

		//Save some properties
		j["type"] = GetType();
		j["constant"] = constant;
		j["linear"] = linear;
		j["quadratic"] = quadratic;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j) override;

	//Variables for the DirectionalLight
	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;
};
