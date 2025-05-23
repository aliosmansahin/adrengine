#include "pch.h"
#include "Object.h"

/*
PURPOSE: Initializes the entity
*/
bool Object::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to ObjectParams to use its properties
	auto casted = std::dynamic_pointer_cast<ObjectParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<ObjectParams*>(params)");
		return false;
	}
	this->params = casted;

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void Object::DeleteEntity()
{
}

/*
PURPOSE: Updates the entity
*/
void Object::Update()
{
	/*
		Here we are calculating the real position, rotation and scale from properties.
		We will use these vectors to draw it in the space.
		If the entity has a parent, add the paramters to real* which is currently from parent's real*.
		Otherwise real* will be properties directly
	*/
	if (params.get()) {
		if (params->parent.get()) {
			realPos += glm::vec3(params->x, params->y, params->z);
			realRot += glm::vec3(params->rx, params->ry, params->rz);
			realSca *= glm::vec3(params->sx, params->sy, params->sz);
		}
		else {
			realPos = glm::vec3(params->x, params->y, params->z);
			realRot = glm::vec3(params->rx, params->ry, params->rz);
			realSca = glm::vec3(params->sx, params->sy, params->sz);
		}
	}
}

/*
PURPOSE: Draws the entity from the position of the camera.
	This entity draws a mesh
*/
void Object::Draw(glm::vec3 currentSceneCameraPos)
{
	//Some calculations for transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, realPos);
	model = glm::rotate(model, glm::radians(realRot.x), glm::vec3(1.0, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.y), glm::vec3(0.0, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.z), glm::vec3(0.0, 0.0f, 1.0f));
	model = glm::scale(model, realSca);

	//Send matrices to the shader
	if (ShaderManager::GetInstance().GetCurrentType() == DEPTH) {
		ShaderManager::GetInstance().ApplyTransformMatrix("model", model);
	}
	else if (ShaderManager::GetInstance().GetCurrentType() == SHADER_3D) {
		ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

		glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		/*glm::vec3 lightPos(100.0f, 100.0f, 100.0f);
		glm::vec3 lightColor = glm::uvec3(1.0f, 1.0f, 1.0f);*/
		//glm::vec3 cameraPos = glm::vec3(SceneManager::GetInstance().currentScene->cameraX, SceneManager::GetInstance().currentScene->cameraY, SceneManager::GetInstance().currentScene->cameraZ);
		glm::vec3 cameraPos = glm::vec3(currentSceneCameraPos);

		ShaderManager::GetInstance().ApplyUniformVec3("objectColor", objectColor);
		/*ShaderManager::GetInstance().ApplyUniformVec3("lightColor", lightColor);
		ShaderManager::GetInstance().ApplyUniformVec3("lightPos", lightPos);*/
		ShaderManager::GetInstance().ApplyUniformVec3("viewPos", cameraPos);
	}

	/*
		TODO: Add drawing materials and textures
	*/
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture();*/

	//Draw the mesh
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(params->objVAO);
	glDrawArrays(GL_TRIANGLES, 0, params->objVerticeCount);

	//Disable after drawing
	//glActiveTexture(0);
	/*glBindTexture(GL_TEXTURE_2D, 0);*/
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* Object::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json Object::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;
}