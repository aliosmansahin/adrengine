#include "pch.h"
#include "DirectionalLight.h"

/*
PURPOSE: Initializes the entity
*/
bool DirectionalLight::CreateEntity(std::shared_ptr<IEntityParams> params)
{
	//Cast EntityParams to DirectionalLightParams to use its properties
	auto casted = std::dynamic_pointer_cast<IDirectionalLightParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<DirectionalLightParams*>(params)");
		return false;
	}
	this->params = casted;

	//Create buffers for shadow
	adr_glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

	adr_glGenTextures(1, &depthMap);
	adr_glBindTexture(GL_TEXTURE_2D, depthMap);
	adr_glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	adr_glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	adr_glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	adr_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	adr_glDrawBuffer(GL_NONE);
	adr_glReadBuffer(GL_NONE);
	adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

/*
PURPOSE: Unitializes the entity
*/
void DirectionalLight::DeleteEntity()
{
	//Delete buffers
	adr_glDeleteFramebuffers(1, &depthMapFBO);
	adr_glDeleteTextures(1, &depthMap);
}

/*
PURPOSE: Updates the entity
*/
void DirectionalLight::Update()
{
	//Some variables
	float width = 100.0f;
	float near_plane = 1.0f, far_plane = 1000.0f;

	//Calculate light martix
	glm::vec3 lightDir = glm::normalize(params->GetDirection());
	glm::mat4 lightProjection = glm::ortho(-width, width, -width, width, near_plane, far_plane);
	//lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(-lightDir * 100.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	lightSpaceMatrix = lightProjection * lightView;
}

/*
PURPOSE: Draws the entity from the position of the camera.
	This entity draws nothing
*/
void DirectionalLight::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
std::shared_ptr<IEntityParams> DirectionalLight::GetEntityParams()
{
	return params;
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json DirectionalLight::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}

/*
PURPOSE: Getters for lightSpaceMatrix
*/
ENTITYMANAGER_API glm::mat4 DirectionalLight::GetLightSpaceMatrix()
{
	return lightSpaceMatrix;
}
