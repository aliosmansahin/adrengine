#include "pch.h"
#include "SpotLight.h"

/*
PURPOSE: Initializes the entity
*/
bool SpotLight::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to SpotLightParams to use its properties
	auto casted = std::dynamic_pointer_cast<SpotLightParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<SpotLightParams*>(params)");
		return false;
	}
	this->params = casted;

	//Create buffers for shadow
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

	glGenFramebuffers(1, &depthMapFBO);
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
void SpotLight::DeleteEntity()
{
	//Delete buffers
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
}

/*
PURPOSE: Updates the entity
*/
void SpotLight::Update()
{
	//Some variables
	float aspect = 1.0f;
	float near_plane = 0.1f;
	float far_plane = 1000.0f;

	//Calculate light martix
	glm::vec3 spotPos = realPos;
	glm::vec3 spotDir = glm::normalize(params->direction);

	float fov = glm::degrees(2 * acos(glm::clamp(params->outerCutOff, -1.0f, 1.0f)));
	glm::mat4 lightProjection = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(spotPos, spotPos + spotDir, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

/*
PURPOSE: Draws the entity from the position of the camera.
	This entity draws nothing
*/
void SpotLight::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* SpotLight::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json SpotLight::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}
