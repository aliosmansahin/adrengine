#pragma once

#include "../ILightParams.h"

class IPointLightParams : public virtual ILightParams
{
public:
	virtual ~IPointLightParams() = default;
};

