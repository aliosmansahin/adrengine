#include "pch.h"
#include "Sprite2D.h"
#include "ShaderManager.h"

/*
PURPOSE: Initializes the entity
*/
bool Sprite2D::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to Sprite2DParams to use its properties
	auto casted = std::dynamic_pointer_cast<Sprite2DParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<Sprite2DParams*>(params)");
		return false;
	}
	this->params = casted;

	//Vertices for the texture
	float vertices[] = {
		//X     Y      Z	 U     V
		 0.0f,  0.0f,  0.0f, 0.0f, 0.0f, // top right
		 0.0f,  32.0f, 0.0f, 0.0f, 1.0f, // bottom right
		 32.0f, 32.0f, 0.0f, 1.0f, 1.0f, // bottom left
		 32.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top left 
	};
	//Indices for the texture
	unsigned int indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//Create buffers and bind them with vertices and indices
	adr_glGenBuffers(1, &VBO);
	adr_glGenBuffers(1, &EBO);

	adr_glGenVertexArrays(1, &VAO);
	adr_glBindVertexArray(VAO);

	adr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
	adr_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	adr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	adr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set the vertex attrib pointers (position = 0)
	adr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	adr_glEnableVertexAttribArray(0);

	//Set the vertex attrib pointers (position = 1)
	adr_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	adr_glEnableVertexAttribArray(1);

	//Release buffers
	adr_glBindBuffer(GL_ARRAY_BUFFER, 0);
	adr_glBindVertexArray(0);
	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void Sprite2D::DeleteEntity()
{
	adr_glDeleteBuffers(1, &VBO);
	adr_glDeleteBuffers(1, &EBO);
	adr_glDeleteVertexArrays(1, &VAO);
}

/*
PURPOSE: Updates the entity
*/
void Sprite2D::Update()
{
}

/*
PURPOSE: Draws the entity.
	This entity draws a texture
*/
void Sprite2D::Draw(glm::vec3 currentSceneCameraPos)
{
	//Some calculations for transformation
	//3d Y and 2d Y are reversed from each other
	//In 3d scene we are rotating it 180 degrees
	float rotate = 0.0f;
	if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		rotate += 180.0f;
		realSca /= 32.0f;
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, realPos);
	model = glm::rotate(model, glm::radians(realRot.x + rotate), glm::vec3(1.0, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.y), glm::vec3(0.0, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.z), glm::vec3(0.0, 0.0f, 1.0f));
	model = glm::scale(model, realSca);

	//Send the transformation matrix to the shader
	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

	//Set the texture
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, params->texture);
	if(ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_2D)
		ShaderManager::GetInstance().ApplyTexture("texture1");
	else if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		//SET THE SHADER TO DRAW BLENDING TEXTURE
		ShaderManager::GetInstance().ApplyUniformBool("isBlending", true);
		ShaderManager::GetInstance().ApplyTexture("objTexture");
	}

	//Draw the texture
	adr_glBindVertexArray(VAO);
	adr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);
	adr_glBindVertexArray(0);
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* Sprite2D::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json Sprite2D::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;

}
