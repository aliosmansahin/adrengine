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

	physical = std::make_shared<Physical>();
	collisionShape = std::make_shared<CollisionShape>();

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
	lastPos = realPos;
	if (physical) {
		if (physical->IsPhysical()) {
			//physical->ApplyForce(glm::vec3(1.0f, 0.0f, 0.0f));

			physical->Update(Timer::GetDeltaTime());

			glm::vec3 newPos = params->GetPosition() + physical->GetVelocity();
			params->SetRuntimePosition(newPos);
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
	if (ShaderManager::GetInstance().GetCurrentType() == Utils::DEPTH) {
		ShaderManager::GetInstance().ApplyUniformMatrix("model", model);
	}
	else if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		ShaderManager::GetInstance().ApplyUniformMatrix("uModel", model);

		glm::vec3 cameraPos = glm::vec3(currentSceneCameraPos);
		ShaderManager::GetInstance().ApplyUniformVec3("viewPos", cameraPos);
	}

	//Draw the mesh
	if (params->mesh) {
		auto& objects = params->mesh->objects;

		//Each ObjectMtl
		for (auto& object : objects) {
			//Send material to the shader
			auto& material = object->material;

			//SET THE SHADER TO DRAW OBJECTS
			ShaderManager::GetInstance().ApplyUniformBool("isBlending", false);

			//ENABLE GAMMA CORRECTION
			ShaderManager::GetInstance().ApplyUniformBool("useGammaCorrection", true);
			
			//TEXTURE
			bool hasTexture = !material.map_Kd.empty() && (material.diffuseTexture != 1);
			ShaderManager::GetInstance().ApplyUniformBool("hasTexture", hasTexture);

			if (hasTexture) {
				//Activate diffuse texture
				adr_glActiveTexture(GL_TEXTURE0);
				adr_glBindTexture(GL_TEXTURE_2D, material.diffuseTexture);
				ShaderManager::GetInstance().ApplyTexture("objTexture");
			}
			else {
				//DIFFUSE
				ShaderManager::GetInstance().ApplyUniformVec3("materialDiffuse", material.Kd);
			}

			//AMBIENT
			ShaderManager::GetInstance().ApplyUniformVec3("materialAmbient", material.Ka * 0.2f);

			//SPECULAR
			ShaderManager::GetInstance().ApplyUniformVec3("materialSpecular", material.Ks);
			
			//SHININESS
			ShaderManager::GetInstance().ApplyUniformFloat("materialShininess", material.Ns);

			//SHININESS
			ShaderManager::GetInstance().ApplyUniformFloat("materialShininess", material.Ns);

			//OPACITY
			ShaderManager::GetInstance().ApplyUniformFloat("materialOpacity", material.d);

			//EMISSION
			ShaderManager::GetInstance().ApplyUniformVec3("materialEmission", material.Ke);

			//REFRACTIVE INDEX
			ShaderManager::GetInstance().ApplyUniformFloat("materialRefractiveIndex", material.Ni);

			//ILLUMINATION
			ShaderManager::GetInstance().ApplyUniformInt("materialIllum", material.illum);

			//Draw the object
			adr_glBindVertexArray(object->VAO);
			adr_glDrawArrays(GL_TRIANGLES, 0, object->verticeCount);

			//Disable after drawing
			adr_glActiveTexture(0);
			adr_glBindTexture(GL_TEXTURE_2D, 0);
			adr_glBindVertexArray(0);
		}
	}
}

/*
PURPOSE: Calls physics->ApplyImpulse function
*/
ENTITYMANAGER_API void Object::AddImpulse(glm::vec3 impulse)
{
	physical->ApplyImpulse(impulse);
}

/*
PURPOSE: Resets variables of the physics
*/
ENTITYMANAGER_API void Object::ResetPhysics()
{
	physical->Reset();
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