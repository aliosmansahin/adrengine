#pragma once

#include "../ISprite2DParams.h"

class IFlipBookParams : public virtual ISprite2DParams
{
public:
	virtual ~IFlipBookParams() = default;

	//Getters for parameters
	virtual float GetFrameWait() = 0;
	virtual bool  GetLoop() = 0;

	virtual void SetFrameWait(float frameWait) = 0;
	virtual void SetLoop(bool loop) = 0;
};

