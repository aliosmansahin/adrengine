#pragma once

#include "Sprite2D.h"
#include "FlipBookParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class FlipBook : public Sprite2D
{
public:
	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<EntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	//frames
	ENTITYMANAGER_API void CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight);
	ENTITYMANAGER_API bool IsInspectCreated();

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<Entity> clone() const override {
		return std::make_shared<FlipBook>(*this);
	}

	//properties
	ENTITYMANAGER_API EntityParams* GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
private:
	//properties
	std::shared_ptr<FlipBookParams> params;

	//inspector framebuffer
	unsigned int inspectFrameBuffer = -1;
	unsigned int inspectRenderBuffer = -1;
	unsigned int inspectTexture = -1;
};