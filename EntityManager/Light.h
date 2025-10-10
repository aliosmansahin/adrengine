#pragma once

#include "Entity.h"
#include "LightParams.h"

#include "interfaces/IEntity/ILight/ILight.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Light : public virtual Entity, public virtual ILight
{
public:
	ENTITYMANAGER_API int GetIndex() override {
		return index;
	};
	ENTITYMANAGER_API void SetIndex(int newIndex) override {
		index = newIndex;
	}
	ENTITYMANAGER_API unsigned int GetDepthMapFBO() override {
		return depthMapFBO;
	}
	ENTITYMANAGER_API unsigned int GetDepthMap() override {
		return depthMap;
	}

protected:
	//light and shadow
	unsigned int depthMapFBO;
	unsigned int depthMap;

	int index;
};

