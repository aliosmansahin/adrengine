#pragma once

#include "../ILightParams.h"

class IDirectionalLightParams : public virtual ILightParams
{
public:
	virtual ~IDirectionalLightParams() = default;
};

