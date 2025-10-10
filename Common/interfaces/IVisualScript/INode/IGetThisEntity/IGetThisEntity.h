#pragma once

#include <memory>

class IEntity;

class IGetThisEntity
{
public:
	virtual void SetEntity(std::shared_ptr<IEntity> entity) = 0;
};

