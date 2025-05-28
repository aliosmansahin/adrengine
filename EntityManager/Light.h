#pragma once

#include "Entity.h"
#include "LightParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Light : public Entity
{
public:
	//light and shadow
	unsigned int depthMapFBO;
	unsigned int depthMap;

	int index;
};

