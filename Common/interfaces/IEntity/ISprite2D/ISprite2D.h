#pragma once

#include "../IEntity.h"
#include "ISprite2DParams.h"

class ISprite2D : public virtual IEntity
{
public:
	virtual ~ISprite2D() = default;
};

