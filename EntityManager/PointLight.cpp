#include "pch.h"
#include "PointLight.h"

/*
PURPOSE: Initializes the entity
*/
bool PointLight::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to PointLightParams to use its properties
	auto casted = std::dynamic_pointer_cast<PointLightParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<PointLightParams*>(params)");
		return false;
	}
	this->params = casted;

	//Create buffers for shadow
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	/*GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}*/

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0); // <- Tüm yüzler için
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void PointLight::DeleteEntity()
{
	//Delete buffers
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
}

/*
PURPOSE: Updates the entity
*/
void PointLight::Update()
{
	//some variables
	float aspect = 1.0f;
	float near_plane = 0.1f;

	//Calculate shadow matrices
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
	glm::vec3 pos = realPos;

	shadowTransforms.reserve(6);
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
	shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
}

/*
PURPOSE: Draws the entity from the position of the camera.
	This entity draws nothing
*/
void PointLight::Draw(glm::vec3 currentSceneCameraPos)
{
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* PointLight::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json PointLight::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}
