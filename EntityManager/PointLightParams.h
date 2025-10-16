#pragma once

#include "LightParams.h"

#include "interfaces/IEntity/ILight/IPointLight/IPointLightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class PointLightParams : public virtual LightParams, public virtual IPointLightParams
{
public:
	ENTITYMANAGER_API virtual ~PointLightParams() = default;

	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "PointLight";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
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
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	//Getters for parameters
	ENTITYMANAGER_API glm::vec3 GetDirection() override {
		return glm::vec3(0.0f);
	}
	ENTITYMANAGER_API float GetOuterCutOff() override {
		return 0.0f;
	}
	ENTITYMANAGER_API float GetCutOff() override {
		return 0.0f;
	}
	ENTITYMANAGER_API float GetConstant() override {
		return constant;
	}
	ENTITYMANAGER_API float GetLinear() override {
		return linear;
	}
	ENTITYMANAGER_API float GetQuadratic() override {
		return quadratic;
	}

	//Setters for parameters
	ENTITYMANAGER_API void SetDirection(glm::vec3 direction) override {}
	ENTITYMANAGER_API void SetOuterCutOff(float outerCutOff) override {}
	ENTITYMANAGER_API void SetCutOff(float cutOff) override {}

	ENTITYMANAGER_API void SetConstant(float constant) override {
		this->constant = constant;
	}
	ENTITYMANAGER_API void SetLinear(float linear) override {
		this->linear = linear;
	}
	ENTITYMANAGER_API void SetQuadratic(float quadratic) override {
		this->quadratic = quadratic;
	}

private:
	//Variables for the DirectionalLight
	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;
};
