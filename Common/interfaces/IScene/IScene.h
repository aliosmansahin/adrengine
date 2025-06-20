#pragma once

#include "../IEntityManager/IEntityManager.h"

class IScene
{
public:
	virtual IEntityManager* GetEntityManager() = 0;
	virtual ~IScene() = default;
};