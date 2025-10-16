#pragma once

#include "../ILightParams.h"

class ISpotLightParams : public virtual ILightParams
{
public:
	virtual ~ISpotLightParams() = default;
};

