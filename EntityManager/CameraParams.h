#pragma once

#include "EntityParams.h"

#include "interfaces/IEntity/ICamera/ICameraParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class CameraParams : public virtual EntityParams, public virtual ICameraParams
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
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
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
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	//projectionType
	ENTITYMANAGER_API CameraProjection GetProjectionType() override {
		return projectionType;
	};
	ENTITYMANAGER_API void SetProjectionType(CameraProjection newProjectionType) override {
		projectionType = newProjectionType;
	}

	//Getter for fov
	ENTITYMANAGER_API float GetFOV() override {
		return fov;
	};
	//Setter for fov
	ENTITYMANAGER_API void SetFOV(float fov) override {
		this->fov = fov;
	};

private:
	//Variables for the Camera
	CameraProjection projectionType = CameraProjection::PERPECTIVE;
	float fov = 70.0f;
};