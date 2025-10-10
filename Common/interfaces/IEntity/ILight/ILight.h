#pragma once

#include "../IEntity.h"

class ILight : public virtual IEntity
{
public:
	virtual int  GetIndex() = 0;
	virtual void SetIndex(int newIndex) = 0;

	virtual unsigned int GetDepthMapFBO() = 0;
	virtual unsigned int GetDepthMap() = 0;
};

