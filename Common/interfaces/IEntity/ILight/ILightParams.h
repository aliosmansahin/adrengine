#pragma once

#include "../IEntityParams.h"

class ILightParams : public virtual IEntityParams
{
public:
	virtual ~ILightParams() = default;

	//Getters for parameters
	virtual glm::vec3 GetColor() = 0;
	virtual glm::vec3 GetDirection() = 0;
	virtual float	  GetOuterCutOff() = 0;
	virtual float	  GetCutOff() = 0;
	virtual float	  GetConstant() = 0;
	virtual float	  GetLinear() = 0;
	virtual float	  GetQuadratic() = 0;


	//Setters for parameters
	virtual void SetColor(glm::vec3 color) = 0;
	virtual void SetDirection(glm::vec3 direction) = 0;
	virtual void SetOuterCutOff(float outerCutOff) = 0;
	virtual void SetCutOff(float cutOff) = 0;
	virtual void SetConstant(float constant) = 0;
	virtual void SetLinear(float linear) = 0;
	virtual void SetQuadratic(float quadratic) = 0;
};

