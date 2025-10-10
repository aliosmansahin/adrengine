#pragma once

#include "../ISprite2D.h"
#include "IFlipBookParams.h"

#include "IFlipBookFrame/IFlipBookFrame.h"

class IFlipBook : public virtual ISprite2D
{
public:
	virtual void CreateFrames(float textureWidth, float textureHeight, float frameWidth, float frameHeight) = 0;
	virtual void CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight) = 0;
	virtual bool IsInspectCreated() = 0;
	virtual void UpdateInspect() = 0;
	virtual void DrawInspect(int width, int height, int tileW, int tileH) = 0;
	virtual unsigned int GetInspectTexture() = 0;
	virtual std::vector<std::pair<bool, std::shared_ptr<IFlipBookFrame>>>& GetCreatedFrames() = 0;
	virtual std::vector<std::shared_ptr<IFlipBookFrame>>& GetFrames() = 0;
	virtual IFlipBookFrame* GetCurrentFrame() = 0;
	virtual void StartFlipBook() = 0;
	virtual void RestartFlipBook() = 0;

	virtual std::pair<int, int> GetFrameSize() = 0;

	virtual void FromJson(nlohmann::json json) = 0;
};

