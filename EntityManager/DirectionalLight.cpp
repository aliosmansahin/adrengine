#include "pch.h"
#include "DirectionalLight.h"

/*
PURPOSE: Initializes the entity
*/
bool DirectionalLight::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to DirectionalLightParams to use its properties
	auto casted = std::dynamic_pointer_cast<DirectionalLightParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<DirectionalLightParams*>(params)");
		return false;
	}
	this->params = casted;

	//Create buffers for shadow
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

/*
PURPOSE: Unitializes the entity
*/
void DirectionalLight::DeleteEntity()
{
	//Delete buffers
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
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
	glm::vec3 lightDir = glm::normalize(params->direction);
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
EntityParams* DirectionalLight::GetEntityParams()
{
    return params.get();
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
