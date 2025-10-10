#pragma once

#include "LightParams.h"

#include "interfaces/IEntity/ILight/ISpotLight/ISpotLightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class SpotLightParams : public virtual LightParams , public virtual ISpotLightParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "SpotLight";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
		return std::make_shared<SpotLightParams>(*this);
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
		j["cut-off"] = cutOff;
		j["outer-cut-off"] = outerCutOff;
		j["constant"] = constant;
		j["linear"] = linear;
		j["quadratic"] = quadratic;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	//Getters for parameters
	ENTITYMANAGER_API glm::vec3 GetDirection() override {
		return direction;
	}
	ENTITYMANAGER_API float GetOuterCutOff() override {
		return outerCutOff;
	}
	ENTITYMANAGER_API float GetCutOff() override {
		return cutOff;
	}
	ENTITYMANAGER_API float	GetConstant() override {
		return constant;
	};
	ENTITYMANAGER_API float	GetLinear() override {
		return linear;
	};
	ENTITYMANAGER_API float	GetQuadratic() override {
		return quadratic;
	};

	//Setters for parameters
	ENTITYMANAGER_API void SetDirection(glm::vec3 direction) override {
		this->direction = direction;
	}
	ENTITYMANAGER_API void SetOuterCutOff(float outerCutOff) override {
		this->outerCutOff = outerCutOff;
	}
	ENTITYMANAGER_API void SetCutOff(float cutOff) override {
		this->cutOff = cutOff;
	}
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
	/*
		Variables for the lights
		Lights will use them as they need
	*/
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(17.5f));

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
