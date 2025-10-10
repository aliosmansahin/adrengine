#pragma once

#include "../IEntityParams.h"

class ISprite2DParams : public virtual IEntityParams
{
public:
	//Getters for the texture
	virtual unsigned int GetTexture() = 0;
	virtual std::string GetTextureId() = 0;

	//Setters for the texture
	virtual void SetTexture(unsigned int texture) = 0;
	virtual void SetTextureId(std::string textureId) = 0;
	
	virtual ~ISprite2DParams() = default;
};

