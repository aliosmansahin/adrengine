#include "pch.h"
#include "FlipBook.h"

/*
PURPOSE: Initializes the entity
*/
ENTITYMANAGER_API bool FlipBook::CreateEntity(std::shared_ptr<EntityParams> params)
{
    //Cast EntityParams to FlipBookParams to use its properties
    auto casted = std::dynamic_pointer_cast<FlipBookParams>(params);
    if (!casted) {
        Logger::Log("E", "Casting failed at dynamic_cast<FlipBookParams*>(params)");
        return false;
    }
    this->params = casted;

    return true;
}

/*
PURPOSE: Unitializes the entity
*/
ENTITYMANAGER_API void FlipBook::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
ENTITYMANAGER_API void FlipBook::Update()
{
    
}

/*
PURPOSE: Draws the entity.
    This entity draws a sprite from a sprite array which is being displayed
*/
ENTITYMANAGER_API void FlipBook::Draw(glm::vec3 currentSceneCameraPos)
{
}

ENTITYMANAGER_API void FlipBook::CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight)
{
    //------ RELEASING ------
	
	//Delete old Buffers
	if (inspectFrameBuffer != -1) {
		adr_glDeleteFramebuffers(1, &inspectFrameBuffer);
		inspectFrameBuffer = -1;
	}
	if (inspectRenderBuffer != -1) {
		adr_glDeleteRenderbuffers(1, &inspectRenderBuffer);
		inspectRenderBuffer = -1;
	}
	if (inspectTexture != -1) {
		adr_glDeleteTextures(1, &inspectTexture);
		inspectTexture = -1;
	}

	//------ FRAME BUFFER ------

	//frame buffer
	adr_glGenFramebuffers(1, &inspectFrameBuffer);
	adr_glBindFramebuffer(GL_FRAMEBUFFER, inspectFrameBuffer);

	//generate a texture and bind it to frame buffer
	adr_glGenTextures(1, &inspectTexture);
	adr_glBindTexture(GL_TEXTURE_2D, inspectTexture);
	adr_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	adr_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inspectTexture, 0);

	//check the status of frame buffer
	if (adr_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Log("E", "Framebuffer is not complete");

	//generate render buffer and bind it to the frame buffer
	adr_glGenRenderbuffers(1, &inspectRenderBuffer);
	adr_glBindRenderbuffer(GL_RENDERBUFFER, inspectRenderBuffer);
	adr_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)textureWidth, (GLsizei)textureHeight);
	adr_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, inspectRenderBuffer);

	//release all buffers
	adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);
	adr_glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ENTITYMANAGER_API bool FlipBook::IsInspectCreated()
{
	if (inspectFrameBuffer == -1)
		return false;
	if (inspectRenderBuffer == -1)
		return false;
	if (inspectTexture == -1)
		return false;
	return true;
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
ENTITYMANAGER_API EntityParams* FlipBook::GetEntityParams()
{
    return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
    "Entity" creates entity json, "Object" creates object json.
*/
ENTITYMANAGER_API nlohmann::json FlipBook::ToJson()
{
	nlohmann::json j; //For the entity

	if (params) {
		j = params->ToJson();
	}

	return j;
}
