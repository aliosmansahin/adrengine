#pragma once

#include "../IEntity/IEntity.h"

#include <memory>
#include <string>

class IEntityManager
{
public:
	virtual IEntity* GetEntityById(std::string id) = 0;
	virtual ~IEntityManager() = default;
};