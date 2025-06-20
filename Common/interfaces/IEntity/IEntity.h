#pragma once

#include "IEntityParams.h"

class IEntity
{
public:
	virtual IEntityParams* GetEntityParams() = 0;
	virtual ~IEntity() = default;
};