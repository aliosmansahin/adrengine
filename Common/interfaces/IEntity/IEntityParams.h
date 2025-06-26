#pragma once

#include <glm/glm.hpp>

class IEntityParams
{
public:
	virtual glm::vec3 GetPosition() const = 0;
	virtual void	  SetRuntimePosition(glm::vec3 vector) = 0;
	virtual glm::vec3 GetRotation() const = 0;
	virtual void	  SetRuntimeRotation(glm::vec3 vector) = 0;
	virtual ~IEntityParams() = default;
};

