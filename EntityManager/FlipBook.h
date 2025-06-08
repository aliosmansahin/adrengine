#pragma once

#include "Sprite2D.h"
#include "FlipBookParams.h"

#include "utils/Utils.h"
#include "ShaderManager.h"
#include "Timer.h"

#include "FlipBookFrame.h"

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
	ENTITYMANAGER_API void CreateFrames(float textureWidth, float textureHeight, float frameWidth, float frameHeight);
	ENTITYMANAGER_API void CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight);
	ENTITYMANAGER_API bool IsInspectCreated();
	ENTITYMANAGER_API void UpdateInspect();
	ENTITYMANAGER_API void DrawInspect(int width, int height, int tileW, int tileH);
	ENTITYMANAGER_API unsigned int GetInspectTexture();
	ENTITYMANAGER_API std::vector<std::pair<bool, std::shared_ptr<FlipBookFrame>>>& GetCreatedFrames();
	ENTITYMANAGER_API std::vector<std::shared_ptr<FlipBookFrame>>& GetFrames();
	ENTITYMANAGER_API FlipBookFrame* GetCurrentFrame();
	ENTITYMANAGER_API void StartFlipBook();

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
	ENTITYMANAGER_API void FromJson(nlohmann::json json);
public:
	int frameWidth = 0;
	int frameHeight = 0;
	float frameWait = 0.1f; //Seconds, duration between each frame //TODO: user will control it from the entity properies window
	bool loop = true; //TODO: user will control it from the entity properties window
	bool ended = false;
private:
	//properties
	std::shared_ptr<FlipBookParams> params;

	//inspector framebuffer
	unsigned int inspectFrameBuffer = -1;
	unsigned int inspectRenderBuffer = -1;
	unsigned int inspectTexture = -1;

	//drawing flipbook
	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;

	//createdFrames
	std::vector<std::pair<bool, std::shared_ptr<FlipBookFrame>>> createdFrames;

	//frames
	std::vector<std::shared_ptr<FlipBookFrame>> frames;
	int currentIndex = -1;

	float lastTime = Timer::GetCurTime();
};