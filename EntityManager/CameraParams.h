#pragma once

#include "EntityParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

enum CameraProjection {
	PERPECTIVE,
	ORTHOGRAPHIC
};

class CameraParams : public EntityParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "Camera";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<EntityParams> clone() const override {
		return std::make_shared<CameraParams>(*this);
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
		j["projection-type"] = projectionType;
		j["fov"] = fov;

		//Save the type
		j["type"] = GetType();

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir) override;

	//Variables for the Camera
	CameraProjection projectionType = CameraProjection::PERPECTIVE;
	float fov = 70.0f;
};