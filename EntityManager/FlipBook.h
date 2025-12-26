#pragma once

#include "Sprite2D.h"
#include "FlipBookParams.h"

#include "utils/Utils.h"
#include "ShaderManager.h"
#include "Timer.h"

#include "FlipBookFrame.h"

#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBook.h"
#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBookFrame/IFlipBookFrame.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class FlipBook : public virtual Sprite2D, public virtual IFlipBook
{
public:
	ENTITYMANAGER_API virtual ~FlipBook() = default;

	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	//frames
	ENTITYMANAGER_API void CreateFrames(float textureWidth, float textureHeight, float frameWidth, float frameHeight) override;
	ENTITYMANAGER_API void CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight) override;
	ENTITYMANAGER_API bool IsInspectCreated() override;
	ENTITYMANAGER_API void UpdateInspect() override;
	ENTITYMANAGER_API void DrawInspect(int width, int height, int tileW, int tileH) override;
	ENTITYMANAGER_API unsigned int GetInspectTexture() override;
	ENTITYMANAGER_API std::vector<std::pair<bool, std::shared_ptr<IFlipBookFrame>>>& GetCreatedFrames() override;
	ENTITYMANAGER_API std::vector<std::shared_ptr<IFlipBookFrame>>& GetFrames() override;
	ENTITYMANAGER_API IFlipBookFrame* GetCurrentFrame() override;
	ENTITYMANAGER_API void StartFlipBook() override;
	ENTITYMANAGER_API void RestartFlipBook() override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<FlipBook>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;
	ENTITYMANAGER_API void FromJson(nlohmann::json json) override;

	//Getters
	ENTITYMANAGER_API std::pair<int, int> GetFrameSize() override { return { frameWidth, frameHeight }; };
private:
	//is the frame ended
	bool ended = false;
private:
	//properties
	std::shared_ptr<IFlipBookParams> params;

	//frame size
	int frameWidth = 0;
	int frameHeight = 0;

	//inspector framebuffer
	FramebufferProvider* inspectFramebuffer = nullptr;

	//createdFrames
	std::vector<std::pair<bool, std::shared_ptr<IFlipBookFrame>>> createdFrames;

	//frames
	std::vector<std::shared_ptr<IFlipBookFrame>> frames;
	int currentIndex = -1;

	float lastTime = Timer::GetCurTime();
};