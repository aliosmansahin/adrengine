#pragma once

#include "Sprite2DParams.h"

#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBookParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class FlipBookParams : public virtual Sprite2DParams, public virtual IFlipBookParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "FlipBook";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
		return std::make_shared<FlipBookParams>(*this);
	}

	/*
	PURPOSE: Creates json content from properties of the entity
	*/
	ENTITYMANAGER_API nlohmann::json ToJson() override {
		/*
			Its kind of "super" function.
			Which calls the function of the base class.
			In this function, we are overriding the function, so we need it
		*/
		auto j = EntityParams::ToJson();

		//Save the id of the texture
		if (!textureId.empty())
			j["texture-id"] = textureId;

		//Save the type
		j["type"] = GetType();
		j["frame-wait"] = frameWait;
		j["loop"] = loop;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	/*
	PURPOSE: Returns frameWait
	*/
	ENTITYMANAGER_API float GetFrameWait() override {
		return frameWait;
	}

	/*
	PURPOSE: Returns loop
	*/
	ENTITYMANAGER_API bool GetLoop() override {
		return loop;
	}

	/*
	PURPOSE: Sets frameWait
	*/
	ENTITYMANAGER_API void SetFrameWait(float frameWait) override {
		this->frameWait = frameWait;
	}

	/*
	PURPOSE: Returns loop
	*/
	ENTITYMANAGER_API void SetLoop(bool loop) override {
		this->loop = loop;
	}

private:
	//showing frames
	float frameWait = 0.1f; //Seconds, duration between each frame
	bool loop = true;
};