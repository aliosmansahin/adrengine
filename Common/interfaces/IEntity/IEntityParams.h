#pragma once

#include <glm/glm.hpp>

class IEntityParams
{
public:
	virtual glm::vec3 GetPosition() const = 0;
	virtual ~IEntityParams() = default;
};

