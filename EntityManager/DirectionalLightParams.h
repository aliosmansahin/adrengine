#pragma once

#include "LightParams.h"

#include "interfaces/IEntity/ILight/IDirectionalLight/IDirectionalLightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class DirectionalLightParams : public virtual LightParams, public virtual IDirectionalLightParams
{
public:
	ENTITYMANAGER_API virtual ~DirectionalLightParams() = default;

	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "DirectionalLight";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
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
		auto j = LightParams::ToJson();

		//Save some properties
		j["type"] = GetType();
		j["direction-x"] = direction.x;
		j["direction-y"] = direction.y;
		j["direction-z"] = direction.z;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	//Getters for parameters
	ENTITYMANAGER_API glm::vec3 GetDirection() override {
		return direction;
	}
	ENTITYMANAGER_API float GetOuterCutOff() override {
		return 0.0f;
	}
	ENTITYMANAGER_API float GetCutOff() override {
		return 0.0f;
	}
	ENTITYMANAGER_API float	GetConstant() override {
		return 0.0f;
	};
	ENTITYMANAGER_API float	GetLinear() override {
		return 0.0f;
	};
	ENTITYMANAGER_API float	GetQuadratic() override {
		return 0.0f;
	};

	ENTITYMANAGER_API void SetDirection(glm::vec3 direction) override {
		this->direction = direction;
	}
	ENTITYMANAGER_API void SetOuterCutOff(float outerCutOff) override {}
	ENTITYMANAGER_API void SetCutOff(float cutOff) override {}
	ENTITYMANAGER_API void SetConstant(float constant) override {};
	ENTITYMANAGER_API void SetLinear(float linear) override {};
	ENTITYMANAGER_API void SetQuadratic(float quadratic) override {};

private:
	//Variables for the DirectionalLight
	glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f);
};